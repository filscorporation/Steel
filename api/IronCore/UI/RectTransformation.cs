using System.Runtime.CompilerServices;

namespace Iron
{
    public class RectTransformation : Transformation
    {
        public Vector2 AnchorMin
        {
            get
            {
                GetAnchorMin_Internal(Entity.ID, out Vector2 anchor);
                return anchor;
            }
            set => SetAnchorMin_Internal(Entity.ID, ref value);
        }

        public Vector2 AnchorMax
        {
            get
            {
                GetAnchorMax_Internal(Entity.ID, out Vector2 anchor);
                return anchor;
            }
            set => SetAnchorMax_Internal(Entity.ID, ref value);
        }

        public Vector3 AnchoredPosition
        {
            get => GetAnchoredPosition_Internal(Entity.ID);
            set => SetAnchoredPosition_Internal(Entity.ID, value);
        }
        
        public Vector2 OffsetMin
        {
            get
            {
                GetOffsetMin_Internal(Entity.ID, out Vector2 offset);
                return offset;
            }
            set => SetOffsetMin_Internal(Entity.ID, ref value);
        }

        public Vector2 OffsetMax
        {
            get
            {
                GetOffsetMax_Internal(Entity.ID, out Vector2 offset);
                return offset;
            }
            set => SetOffsetMax_Internal(Entity.ID, ref value);
        }

        public Vector2 Pivot
        {
            get
            {
                GetPivot_Internal(Entity.ID, out Vector2 pivot);
                return pivot;
            }
            set => SetPivot_Internal(Entity.ID, ref value);
        }

        public Vector2 Size
        {
            get
            {
                GetSize_Internal(Entity.ID, out Vector2 size);
                return size;
            }
            set => SetSize_Internal(Entity.ID, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetAnchorMin_Internal(ulong entityID, out Vector2 anchor);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAnchorMin_Internal(ulong entityID, ref Vector2 anchor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetAnchorMax_Internal(ulong entityID, out Vector2 anchor);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAnchorMax_Internal(ulong entityID, ref Vector2 anchor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetAnchoredPosition_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAnchoredPosition_Internal(ulong entityID, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetOffsetMin_Internal(ulong entityID, out Vector2 offset);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetOffsetMin_Internal(ulong entityID, ref Vector2 offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetOffsetMax_Internal(ulong entityID, out Vector2 offset);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetOffsetMax_Internal(ulong entityID, ref Vector2 offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetPivot_Internal(ulong entityID, out Vector2 pivot);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPivot_Internal(ulong entityID, ref Vector2 pivot);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetSize_Internal(ulong entityID, out Vector2 size);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSize_Internal(ulong entityID, ref Vector2 size);
    }
}