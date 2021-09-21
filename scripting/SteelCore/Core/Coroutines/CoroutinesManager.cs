using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace Steel
{
    /// <summary>
    /// Starts, manages and updates coroutines
    /// </summary>
    internal static class CoroutinesManager
    {
        private static readonly Dictionary<uint, LinkedList<Coroutine>> coroutines = new Dictionary<uint, LinkedList<Coroutine>>();

        private static void Update()
        {
            float deltaTime = Time.DeltaTime;
            LinkedList<uint> toRemove = new LinkedList<uint>();

            foreach (var pair in coroutines)
            {
                var currentNode = pair.Value.First;
                while (currentNode != null)
                {
                    var temp = currentNode.Next;

                    // Checking IsActiveByID every frame can be expensive, but only on very big coroutines number,
                    // but that is not their purpose - coroutines are not designed to replace all actions,
                    // it's just useful tool to delay some actions
                    if (!Entity.IsActiveByID(currentNode.Value.Owner) || !Process(currentNode.Value, deltaTime))
                    {
                        currentNode.Value.IsDestroyed = true;
                        pair.Value.Remove(currentNode);
                        if (!pair.Value.Any())
                            toRemove.AddLast(pair.Key);
                    }
                
                    currentNode = temp;
                }
            }

            foreach (uint entityID in toRemove)
            {
                coroutines.Remove(entityID);
            }
        }

        private static bool Process(Coroutine coroutine, float deltaTime)
        {
            object yieldResult = coroutine.Enumerator.Current;
            switch (yieldResult)
            {
                case WaitForSeconds waitForSeconds:
                    waitForSeconds.Duration -= deltaTime;
                    if (waitForSeconds.Duration > 0)
                        return true;
                    break;
                case WaitForSecondsUnscaled waitForSecondsUnscaled:
                    waitForSecondsUnscaled.Duration -= deltaTime / Time.TimeScale;
                    if (waitForSecondsUnscaled.Duration > 0)
                        return true;
                    break;
                case WaitWhile waitWhile:
                    if (waitWhile.Predicate())
                        return true;
                    break;
            }

            return coroutine.Enumerator.MoveNext();
        }

        internal static Coroutine AddCoroutine(uint owner, IEnumerator enumerator)
        {
            enumerator.MoveNext();
            Coroutine coroutine = new Coroutine(owner, enumerator);
            if (!coroutines.ContainsKey(owner))
                coroutines[owner] = new LinkedList<Coroutine>();
            coroutine.Node = coroutines[owner].AddLast(coroutine);

            return coroutine;
        }

        internal static void StopCoroutine(Coroutine coroutine)
        {
            if (coroutine.IsDestroyed || !coroutines.ContainsKey(coroutine.Owner))
                return;

            coroutine.IsDestroyed = true;
            coroutines[coroutine.Owner].Remove(coroutine.Node);
            if (!coroutines[coroutine.Owner].Any())
                coroutines.Remove(coroutine.Owner);
        }

        public static void StopAllCoroutines(uint ownerID)
        {
            if (!coroutines.ContainsKey(ownerID))
                return;

            foreach (Coroutine coroutine in coroutines[ownerID])
                coroutine.IsDestroyed = true;
            coroutines.Remove(ownerID);
        }
    }
}