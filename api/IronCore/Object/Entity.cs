using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Iron
{
    public class Entity
    {
        public ulong ID { get; private set; }
        private Dictionary<ulong, ScriptComponent> unmanagedComponentsCache = new Dictionary<ulong, ScriptComponent>();
        
        public Entity()
        {
            ID = CreateNewEntity_Internal();
        }
        
        public T AddComponent<T>() where T : Component, new()
        {
            ulong componentID;
            T component = new T();
            if (component is ScriptComponent script)
            {
                IntPtr pointer = GCHandle.ToIntPtr(GCHandle.Alloc(component));
                componentID = AddScriptComponent_Internal(ID, typeof(T), pointer);
                component.ID = componentID;
                unmanagedComponentsCache[component.ID] = script;
            }
            else
            {
                componentID = AddComponent_Internal(ID, typeof(T));
                component.ID = componentID;
            }
            
            if (componentID == 0)
            {
                return null;
            }
            
            component.Entity = this;
            
            return component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            return GetComponent<T>() != null; // TODO: rework to use internals
        }
        
        public T GetComponent<T>() where T : Component, new()
        {
            ulong componentID;
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                componentID = GetScriptComponent_Internal(ID, typeof(T));
                if (unmanagedComponentsCache.TryGetValue(componentID, out var script))
                {
                    return script as T;
                }
            }
            else
            {
                componentID = GetComponent_Internal(ID, typeof(T));
            }
            
            if (componentID == 0)
                return null;
            
            T component = new T();
            component.Entity = this;
            
            return component;
        }
        
        public bool RemoveComponent<T>() where T : Component
        {
            ulong componentID;
            if (typeof(T).IsSubclassOf(typeof(ScriptComponent)))
            {
                componentID = RemoveScriptComponent_Internal(ID, typeof(T));
                unmanagedComponentsCache.Remove(componentID);
            }
            else
            {
                componentID = RemoveComponent_Internal(ID, typeof(T));
            }
            
            return componentID != 0;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong CreateNewEntity_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong AddComponent_Internal(ulong entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong AddScriptComponent_Internal(ulong entityID, Type type, IntPtr scriptPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetComponent_Internal(ulong entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetScriptComponent_Internal(ulong entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong RemoveComponent_Internal(ulong entityID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong RemoveScriptComponent_Internal(ulong entityID, Type type);
    }
}