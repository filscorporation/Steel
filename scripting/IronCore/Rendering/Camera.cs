using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Component responsible for rendering the scene
    /// </summary>
    public class Camera : Component
    {
        /// <summary>
        /// Camera in the scene, that is set as main
        /// </summary>
        // TODO: not optimal
        public static Camera Main => Entity.GetInternalComponentByEntityID<Camera>(GetEntityWithMainCamera_Internal());

        /// <summary>
        /// Viewport width in world coordinates
        /// </summary>
        public float Width
        {
            get => GetWidth_Internal(Entity.ID);
            set => SetWidth_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Viewport height in world coordinates
        /// </summary>
        public float Height
        {
            get => GetHeight_Internal(Entity.ID);
            set => SetHeight_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Distance to the closest objects that will get drawn
        /// </summary>
        public float NearClippingPlane
        {
            get => GetNearClippingPlane_Internal(Entity.ID);
            set => SetNearClippingPlane_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Maximum distance on which objects will get drawn
        /// </summary>
        public float FarClippingPlane
        {
            get => GetFarClippingPlane_Internal(Entity.ID);
            set => SetFarClippingPlane_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Defines how camera will resize when screen or viewport size get changed
        /// </summary>
        public CameraResizingMode ResizingMode
        {
            get => GetResizingMode_Internal(Entity.ID);
            set => SetResizingMode_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Convert world position to screen coordinates
        /// </summary>
        /// <param name="worldPoint">Point in world coordinates</param>
        /// <returns>Point in screen coordinates</returns>
        public Vector2 WorldToScreenPoint(Vector2 worldPoint)
        {
            WorldToScreenPoint_Internal(Entity.ID, ref worldPoint, out Vector2 screenPoint);
            return screenPoint;
        }
        
        /// <summary>
        /// Convert screen position to world coordinates
        /// </summary>
        /// <param name="screenPoint">Point in screen coordinates</param>
        /// <returns>Point in world coordinates</returns>
        public Vector2 ScreenToWorldPoint(Vector2 screenPoint)
        {
            ScreenToWorldPoint_Internal(Entity.ID, ref screenPoint, out Vector2 worldPoint);
            return worldPoint;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetEntityWithMainCamera_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetWidth_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetWidth_Internal(uint entityID, float width);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetHeight_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetHeight_Internal(uint entityID, float height);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetNearClippingPlane_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetNearClippingPlane_Internal(uint entityID, float distance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetFarClippingPlane_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetFarClippingPlane_Internal(uint entityID, float distance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern CameraResizingMode GetResizingMode_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetResizingMode_Internal(uint entityID, CameraResizingMode mode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void WorldToScreenPoint_Internal(uint entityID, ref Vector2 worldPoint, out Vector2 screenPoint);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ScreenToWorldPoint_Internal(uint entityID, ref Vector2 screenPoint, out Vector2 worldPoint);
    }
}