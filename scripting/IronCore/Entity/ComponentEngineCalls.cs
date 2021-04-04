using System;
using System.Runtime.InteropServices;

namespace Iron
{
    /// <summary>
    /// Internal class used to capture engine calls and redirect them to entities
    /// </summary>
    internal static class ComponentEngineCalls
    {
        private static void FreeScriptHandle(IntPtr componentPtr)
        {
            GCHandle.FromIntPtr(componentPtr).Free();
        }
        
        private static void ComponentOnCreate(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnCreate();
        }
        
        private static void ComponentOnUpdate(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnUpdate();
        }
        
        private static void ComponentOnLateUpdate(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnLateUpdate();
        }
        
        private static void ComponentOnFixedUpdate(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnFixedUpdate();
        }
        
        private static void ComponentOnEnabled(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnEnabled();
        }
        
        private static void ComponentOnDisabled(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnDisabled();
        }
        
        private static void ComponentOnCollisionEnter(IntPtr componentPtr, uint otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnCollisionEnter(collision);
        }
        
        private static void ComponentOnCollisionStay(IntPtr componentPtr, uint otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnCollisionStay(collision);
        }
        
        private static void ComponentOnCollisionExit(IntPtr componentPtr, uint otherEntityID)
        {
            Collision collision = new Collision(new Entity(otherEntityID));
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnCollisionExit(collision);
        }
        
        private static void ComponentOnMouseOver(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseOver();
        }
        
        private static void ComponentOnMouseEnter(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseEnter();
        }
        
        private static void ComponentOnMouseExit(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseExit();
        }
        
        private static void ComponentOnMousePressed(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnMousePressed();
        }
        
        private static void ComponentOnMouseJustPressed(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseJustPressed();
        }
        
        private static void ComponentOnMouseJustReleased(IntPtr componentPtr)
        {
            ((ScriptComponent)GCHandle.FromIntPtr(componentPtr).Target)?.OnMouseJustReleased();
        }
    }
}