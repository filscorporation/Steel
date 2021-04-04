using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Iron
{
    /// <summary>
    /// Base class for all components, that can be attached to <see cref="Entity"/>
    /// </summary>
    public abstract class Component
    {
        internal Component() { }
        
        /// <summary>
        /// Entity this component is attached. Component is always attached to some entity
        /// </summary>
        public Entity Entity { get; set; }
        
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