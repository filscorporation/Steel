using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Iron
{
    /// <summary>
    /// Basic object in the scene, represented by the list of components attached
    /// </summary>
    public class Entity
    {
        internal const uint NULL_ENTITY_ID = 0xffffffff;
        
        /// <summary>
        /// Unique identifier
        /// </summary>
        public uint ID { get; }
        
        /// <summary>
        /// Name
        /// </summary>
        public string Name
        {
            get => GetName_Internal(ID);
            set => SetName_Internal(ID, value);
        }

        internal Entity(uint id)
        {
            ID = id;
        }
        
        /// <summary>
        /// Instantiates new entity in the scene
        /// </summary>
        /// <remarks>Note that UI elements should be created from <see cref="UI"/> class</remarks>
        public Entity()
        {
            ID = CreateNewEntity_Internal();
        }
        
        /// <summary>
        /// Instantiates new entity with a given name in the scene
        /// </summary>
        /// <remarks>Note that UI elements should be created from <see cref="UI"/> class</remarks>
        public Entity(string name)
        {
            ID = CreateNewEntity_Internal2(name);
        }
        
        /// <summary>
        /// Instantiates new entity with a given name and parent in the scene
        /// </summary>
        /// <remarks>Note that UI elements should be created from <see cref="UI"/> class</remarks>
        public Entity(string name, Entity parent)
        {
            ID = CreateNewEntity_Internal3(name, parent?.ID ?? NULL_ENTITY_ID);
        }

        /// <summary>
        /// Gets entity transformation component (will be null for UI elements)
        /// </summary>
        public Transformation Transformation => GetComponent<Transformation>();

        /// <summary>
        /// Is entity active in the scene (including parents active status)
        /// </summary>
        public bool IsActive => GetIsActive_Internal(ID);

        /// <summary>
        /// Is entity active by itself in the scene
        /// </summary>
        public bool IsActiveSelf
        {
            get => GetIsActiveSelf_Internal(ID);
            set => SetIsActiveSelf_Internal(ID, value);
        }

        /// <summary>
        /// Requests to destroy entity in the end of the frame
        /// </summary>
        public void Destroy() => DestroyEntity_Internal(ID);
        
        /// <summary>
        /// Attaches new components to the entity and returns it
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>Attached component or already existing one</returns>
        /// <remarks>Components should be unique for the entity</remarks>
        public T AddComponent<T>() where T : Component, new()
        {
            T component;
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                component = new T();
                IntPtr scriptPointer = GCHandle.ToIntPtr(GCHandle.Alloc(component));
                component.Entity = this;
                
                if (!AddScriptComponent_Internal(ID, typeof(T), scriptPointer))
                {
                    GCHandle.FromIntPtr(scriptPointer).Free();
                    
                    // TODO: can be done in one call
                    return (T)GCHandle.FromIntPtr(GetScriptComponent_Internal(ID, typeof(T))).Target;
                }

                return component;
            }
            
            if (!AddComponent_Internal(ID, typeof(T)))
                return null;

            component = new T();
            component.Entity = this;
            
            return component;
        }

        /// <summary>
        /// Check if entity has component of requested type attached
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>True if there is component of requested type attached</returns>
        public bool HasComponent<T>() where T : Component, new()
        {
            return typeof(T).IsSubclassOf(typeof(ScriptComponent))
                ? HasScriptComponent_Internal(ID, typeof(T))
                : HasComponent_Internal(ID, typeof(T));
        }
        
        /// <summary>
        /// Get component attached to this entity
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>Component or null if there is no component of requested type attached</returns>
        public T GetComponent<T>() where T : Component, new()
        {
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                IntPtr scriptPointer = GetScriptComponent_Internal(ID, typeof(T));
                if (scriptPointer == IntPtr.Zero)
                    return null;
                return (T)GCHandle.FromIntPtr(scriptPointer).Target;
            }

            if (!HasComponent_Internal(ID, typeof(T)))
                return null;
            
            T component = new T();
            component.Entity = this;
            
            return component;
        }
        
        /// <summary>
        /// Removes attached component from this entity
        /// </summary>
        /// <typeparam name="T">Component type</typeparam>
        /// <returns>True if there was component of requested type attached</returns>
        /// <remarks>You should not remove <see cref="Transformation"/> and <see cref="RectTransformation"/> components</remarks>
        public bool RemoveComponent<T>() where T : Component
        {
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                IntPtr scriptPointer = RemoveScriptComponent_Internal(ID, typeof(T));
                return scriptPointer != IntPtr.Zero;
            }

            return RemoveComponent_Internal(ID, typeof(T));
        }

        internal static T GetInternalComponentByEntityID<T>(uint entityID) where T : Component, new()
        {
            if (!HasComponent_Internal(entityID, typeof(T)))
                return null;
            
            T component = new T();
            component.Entity = new Entity(entityID);

            return component;
        }

        /// <summary>
        /// Entity parent in the hierarchy or null if there is none
        /// </summary>
        public Entity Parent
        {
            get
            {
                uint parentEntityID = GetParent_Internal(ID);

                return parentEntityID == NULL_ENTITY_ID ? null : new Entity(parentEntityID);
            }
            set => SetParent_Internal(ID, value?.ID ?? NULL_ENTITY_ID);
        }
        
        /// <summary>
        /// List of entity's children in the hierarchy if there is any
        /// </summary>
        public IEnumerable<Entity> Children
        {
            get { return GetChildren_Internal(ID).Select(childEntityID => new Entity(childEntityID)); }
        }

        /// <summary>
        /// Check if entity already destroyed in the engine
        /// </summary>
        /// <returns>True if component was destroyed</returns>
        public bool IsDestroyed() => IsDestroyed_Internal(ID);

        /// <summary>
        /// Starts coroutine on the current entity
        /// </summary>
        /// <param name="coroutine">Coroutine to run</param>
        /// <returns>Coroutine object</returns>
        public Coroutine StartCoroutine(IEnumerator coroutine)
        {
            return CoroutinesManager.AddCoroutine(ID, coroutine);
        }
        
        /// <summary>
        /// Stops coroutine
        /// </summary>
        /// <param name="coroutine">Coroutine object to stop</param>
        public void StopCoroutine(Coroutine coroutine)
        {
            CoroutinesManager.StopCoroutine(coroutine);
        }
        
        /// <summary>
        /// Stops all coroutines currently running on this entity
        /// </summary>
        public void StopAllCoroutines()
        {
            CoroutinesManager.StopAllCoroutines(ID);
        }
        
        internal static bool IsActiveByID(uint entityID) => GetIsActive_Internal(entityID);

        public static bool operator ==(Entity a, Entity b) => Equals(a, b);
        public static bool operator !=(Entity a, Entity b) => !Equals(a, b);

        public override bool Equals(object obj)
        {
            if (obj is Entity entity)
                return ID == entity.ID;
            
            return false;
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }

        public override string ToString()
        {
            return $"{Name} ({ID})";
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateNewEntity_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateNewEntity_Internal2(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateNewEntity_Internal3(string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetIsActive_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetIsActiveSelf_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetIsActiveSelf_Internal(uint entityID, bool isActiveSelf);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool DestroyEntity_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AddComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AddScriptComponent_Internal(uint entityID, Type type, IntPtr scriptPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasScriptComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr GetScriptComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool RemoveComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr RemoveScriptComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetName_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetName_Internal(uint entityID, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetParent_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetParent_Internal(uint entityID, uint parentEntityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint[] GetChildren_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsDestroyed_Internal(uint entityID);
    }
}