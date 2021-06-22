using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Component representing UI element position, rotation and size in the UI layout
    /// </summary>
    public class RectTransformation : Component
    {
        /// <summary>
        /// Bottom left anchor point
        /// </summary>
        public Vector2 AnchorMin
        {
            get
            {
                GetAnchorMin_Internal(Entity.ID, out Vector2 anchor);
                return anchor;
            }
            set => SetAnchorMin_Internal(Entity.ID, ref value);
        }

        /// <summary>
        /// Top right anchor point
        /// </summary>
        public Vector2 AnchorMax
        {
            get
            {
                GetAnchorMax_Internal(Entity.ID, out Vector2 anchor);
                return anchor;
            }
            set => SetAnchorMax_Internal(Entity.ID, ref value);
        }

        /// <summary>
        /// Position relative to anchors
        /// </summary>
        public Vector2 AnchoredPosition
        {
            get
            {
                GetAnchoredPosition_Internal(Entity.ID, out Vector2 position);
                return position;
            }
            set => SetAnchoredPosition_Internal(Entity.ID, ref value);
        }
        
        /// <summary>
        /// Offset from bottom left anchor
        /// </summary>
        public Vector2 OffsetMin
        {
            get
            {
                GetOffsetMin_Internal(Entity.ID, out Vector2 offset);
                return offset;
            }
            set => SetOffsetMin_Internal(Entity.ID, ref value);
        }
        
        /// <summary>
        /// Offset from top right anchor
        /// </summary>
        public Vector2 OffsetMax
        {
            get
            {
                GetOffsetMax_Internal(Entity.ID, out Vector2 offset);
                return offset;
            }
            set => SetOffsetMax_Internal(Entity.ID, ref value);
        }
        
        /// <summary>
        /// Rect pivot
        /// </summary>
        public Vector2 Pivot
        {
            get
            {
                GetPivot_Internal(Entity.ID, out Vector2 pivot);
                return pivot;
            }
            set => SetPivot_Internal(Entity.ID, ref value);
        }

        /// <summary>
        /// Rect size
        /// </summary>
        public Vector2 Size
        {
            get
            {
                GetSize_Internal(Entity.ID, out Vector2 size);
                return size;
            }
            set => SetSize_Internal(Entity.ID, ref value);
        }

        /// <summary>
        /// Order when element will be drawn in UI, bigger - later
        /// </summary>
        public float SortingOrder => GetSortingOrder_Internal(Entity.ID);

        /// <summary>
        /// Element global rotation
        /// </summary>
        public Vector3 Rotation
        {
            get => GetRotation_Internal(Entity.ID);
            set => SetRotation_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetAnchorMin_Internal(uint entityID, out Vector2 anchor);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAnchorMin_Internal(uint entityID, ref Vector2 anchor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetAnchorMax_Internal(uint entityID, out Vector2 anchor);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAnchorMax_Internal(uint entityID, ref Vector2 anchor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetAnchoredPosition_Internal(uint entityID, out Vector2 position);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAnchoredPosition_Internal(uint entityID, ref Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetOffsetMin_Internal(uint entityID, out Vector2 offset);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetOffsetMin_Internal(uint entityID, ref Vector2 offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetOffsetMax_Internal(uint entityID, out Vector2 offset);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetOffsetMax_Internal(uint entityID, ref Vector2 offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetPivot_Internal(uint entityID, out Vector2 pivot);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPivot_Internal(uint entityID, ref Vector2 pivot);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetSize_Internal(uint entityID, out Vector2 size);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSize_Internal(uint entityID, ref Vector2 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetSortingOrder_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetRotation_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRotation_Internal(uint entityID, Vector3 rotation);
    }
}