using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Base class for all components, that can be attached to <see cref="Entity"/>
    /// </summary>
    public abstract class Component
    {
        internal Component() { }
        
        /// <summary>
        /// Entity this component is attached to. Component is always attached to some entity
        /// </summary>
        public Entity Entity { get; internal set; }
        
        /// <summary>
        /// Gets transformation components attached to entity (will be null for UI elements)
        /// </summary>
        public Transformation Transformation => GetComponent<Transformation>();

        /// <summary>
        /// Check if this components entity has component of requested type
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>True if there is component attached</returns>
        public bool HasComponent<T>() where T : Component, new()
        {
            return Entity.HasComponent<T>();
        }
        
        /// <summary>
        /// Get component attached to the same entity as this
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>Component or null if there is no component of requested type attached</returns>
        public T GetComponent<T>() where T : Component, new()
        {
            return Entity.GetComponent<T>();
        }

        /// <summary>
        /// Returns all active components in the scene of requested type
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>Requested components enumeration</returns>
        public static IEnumerable<T> FindAllOfType<T>() where T : Component, new()
        {
            FindAllOfType_Internal(typeof(T), out Component[] result);
            return result.Select(component => component as T);
        }

        /// <summary>
        /// Starts coroutine on the owner entity
        /// </summary>
        /// <param name="coroutine">Coroutine to run</param>
        /// <returns>Coroutine object</returns>
        public Coroutine StartCoroutine(IEnumerator coroutine)
        {
            return Entity.StartCoroutine(coroutine);
        }
        
        /// <summary>
        /// Stops coroutine
        /// </summary>
        /// <param name="coroutine">Coroutine object to stop</param>
        public void StopCoroutine(Coroutine coroutine)
        {
            Entity.StopCoroutine(coroutine);
        }
        
        /// <summary>
        /// Stops all coroutines currently running on the owner entity
        /// </summary>
        public void StopAllCoroutines()
        {
            Entity.StopAllCoroutines();
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void FindAllOfType_Internal(Type type, out Component[] result);
    }
}