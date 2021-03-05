using System;
using System.Runtime.InteropServices;

namespace Iron
{
    public static class ComponentEngineCalls
    {
        private static void ComponentOnCreate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnCreate();
        }
        
        private static void ComponentOnUpdate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnUpdate();
        }
        
        private static void ComponentOnLateUpdate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnLateUpdate();
        }
        
        private static void ComponentOnFixedUpdate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnFixedUpdate();
        }
        
        private static void ComponentOnCollisionEnter(IntPtr componentPtr, ulong otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnCollisionEnter(collision);
        }
        
        private static void ComponentOnCollisionStay(IntPtr componentPtr, ulong otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnCollisionStay(collision);
        }
        
        private static void ComponentOnCollisionExit(IntPtr componentPtr, ulong otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((Component)GCHandle.FromIntPtr(componentPtr).Target).OnCollisionExit(collision);
        }
    }
}