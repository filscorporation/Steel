using System.Runtime.CompilerServices;

namespace Iron
{
    public class Camera : Component
    {
        // TODO: not optimal
        public static Camera Main => Entity.GetComponentByEntityID<Camera>(GetMainEntityWithMainCamera_Internal());

        public float Width
        {
            get => GetWidth_Internal(Entity.ID);
            set => SetWidth_Internal(Entity.ID, value);
        }

        public float Height
        {
            get => GetHeight_Internal(Entity.ID);
            set => SetHeight_Internal(Entity.ID, value);
        }

        public float NearClippingPlane
        {
            get => GetNearClippingPlane_Internal(Entity.ID);
            set => SetNearClippingPlane_Internal(Entity.ID, value);
        }

        public float FarClippingPlane
        {
            get => GetFarClippingPlane_Internal(Entity.ID);
            set => SetFarClippingPlane_Internal(Entity.ID, value);
        }

        public CameraResizingMode ResizingMode
        {
            get => GetResizingMode_Internal(Entity.ID);
            set => SetResizingMode_Internal(Entity.ID, value);
        }

        public Vector2 WorldToScreenPoint(Vector2 worldPoint)
        {
            WorldToScreenPoint_Internal(Entity.ID, ref worldPoint, out Vector2 screenPoint);
            return screenPoint;
        }

        public Vector2 ScreenToWorldPoint(Vector2 screenPoint)
        {
            ScreenToWorldPoint_Internal(Entity.ID, ref screenPoint, out Vector2 worldPoint);
            return worldPoint;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetMainEntityWithMainCamera_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetWidth_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetWidth_Internal(ulong entityID, float width);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetHeight_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetHeight_Internal(ulong entityID, float height);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetNearClippingPlane_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetNearClippingPlane_Internal(ulong entityID, float distance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetFarClippingPlane_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetFarClippingPlane_Internal(ulong entityID, float distance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern CameraResizingMode GetResizingMode_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetResizingMode_Internal(ulong entityID, CameraResizingMode mode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void WorldToScreenPoint_Internal(ulong entityID, ref Vector2 worldPoint, out Vector2 screenPoint);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ScreenToWorldPoint_Internal(ulong entityID, ref Vector2 screenPoint, out Vector2 worldPoint);
    }
}