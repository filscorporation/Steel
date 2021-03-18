using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Iron
{
    public abstract class Component
    {
        internal Component() { }
        
        public Entity Entity { get; set; }
        
        public Transformation Transformation => GetComponent<Transformation>();

        public bool HasComponent<T>() where T : Component, new()
        {
            return Entity.HasComponent<T>();
        }
        
        public T GetComponent<T>() where T : Component, new()
        {
            return Entity.GetComponent<T>();
        }

        public static IEnumerable<T> FindAllOfType<T>() where T : Component, new()
        {
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                foreach (IntPtr scriptPointer in FindAllScriptsOfType_Internal(typeof(T)))
                {
                    if (scriptPointer == IntPtr.Zero)
                        yield return null;
                    yield return (T)GCHandle.FromIntPtr(scriptPointer).Target;
                }
                yield break;
            }

            foreach (uint componentEntityID in FindAllOfType_Internal(typeof(T)))
            {
                T component = new T();
                component.Entity = new Entity(componentEntityID);
                yield return component;
            }
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint[] FindAllOfType_Internal(Type type);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr[] FindAllScriptsOfType_Internal(Type type);
    }
}