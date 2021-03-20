using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Iron
{
    public class Entity
    {
        internal const uint NULL_ENTITY_ID = 0;
        public uint ID { get; }
        
        public string Name
        {
            get => GetName_Internal(ID);
            set => SetName_Internal(ID, value);
        }

        internal Entity(uint id)
        {
            ID = id;
        }
        
        public Entity()
        {
            ID = CreateNewEntity_Internal();
        }
        
        public Entity(string name)
        {
            ID = CreateNewEntity_Internal2(name);
        }
        
        public Entity(string name, Entity parent)
        {
            ID = CreateNewEntity_Internal3(name, parent?.ID ?? NULL_ENTITY_ID);
        }

        public Transformation Transformation => GetComponent<Transformation>();

        public void Destroy() => DestroyEntity_Internal(ID);
        
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
                    return null;
                }

                return component;
            }
            
            if (!AddComponent_Internal(ID, typeof(T)))
                return null;

            component = new T();
            component.Entity = this;
            
            return component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            return HasComponent_Internal(ID, typeof(T));
        }
        
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
        
        public bool RemoveComponent<T>() where T : Component
        {
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                IntPtr scriptPointer = RemoveScriptComponent_Internal(ID, typeof(T));
                if (scriptPointer == IntPtr.Zero)
                    return false;
                GCHandle.FromIntPtr(scriptPointer).Free();
                return true;
            }

            return RemoveComponent_Internal(ID, typeof(T));
        }

        internal static T GetComponentByEntityID<T>(uint entityID) where T : Component, new()
        {
            if (!HasComponent_Internal(entityID, typeof(T)))
                return null;
            
            T component = new T();
            component.Entity = new Entity(entityID);

            return component;
        }

        public Entity Parent
        {
            get
            {
                uint parentEntityID = GetParent_Internal(ID);

                return parentEntityID == NULL_ENTITY_ID ? null : new Entity(parentEntityID);
            }
            set => SetParent_Internal(ID, value?.ID ?? NULL_ENTITY_ID);
        }
        
        public IEnumerable<Entity> Children
        {
            get { return GetChildren_Internal(ID).Select(childEntityID => new Entity(childEntityID)); }
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
        private static extern bool DestroyEntity_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AddComponent_Internal(uint entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool AddScriptComponent_Internal(uint entityID, Type type, IntPtr scriptPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Internal(uint entityID, Type type);

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
    }
}