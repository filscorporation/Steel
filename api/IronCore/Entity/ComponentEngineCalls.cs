using System;
using System.Runtime.InteropServices;

namespace Iron
{
    public static class ComponentEngineCalls
    {
        private static void ComponentOnCreate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnCreate();
        }
        
        private static void ComponentOnUpdate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnUpdate();
        }
        
        private static void ComponentOnLateUpdate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnLateUpdate();
        }
        
        private static void ComponentOnFixedUpdate(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnFixedUpdate();
        }
        
        private static void ComponentOnCollisionEnter(IntPtr componentPtr, uint otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnCollisionEnter(collision);
        }
        
        private static void ComponentOnCollisionStay(IntPtr componentPtr, uint otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnCollisionStay(collision);
        }
        
        private static void ComponentOnCollisionExit(IntPtr componentPtr, uint otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnCollisionExit(collision);
        }
        
        private static void ComponentOnMouseOver(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseOver();
        }
        
        private static void ComponentOnMouseEnter(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseEnter();
        }
        
        private static void ComponentOnMouseExit(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseExit();
        }
        
        private static void ComponentOnMousePressed(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnMousePressed();
        }
        
        private static void ComponentOnMouseJustPressed(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseJustPressed();
        }
        
        private static void ComponentOnMouseJustReleased(IntPtr componentPtr)
        {
            ((Component)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseJustReleased();
        }
    }
}