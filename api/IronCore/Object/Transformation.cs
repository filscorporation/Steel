﻿using System.Runtime.CompilerServices;
using Iron.Math;

namespace Iron
{
    public class Transformation : Component
    {
        public Vector3 Position
        {
            get => GetPosition_Internal(Entity.ID);
            set => SetPosition_Internal(Entity.ID, value);
        }
        
        public Vector3 Rotation
        {
            get => GetRotation_Internal(Entity.ID);
            set => SetRotation_Internal(Entity.ID, value);
        }
        
        public Vector3 Scale
        {
            get => GetScale_Internal(Entity.ID);
            set => SetScale_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetPosition_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPosition_Internal(ulong entityID, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetRotation_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRotation_Internal(ulong entityID, Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetScale_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetScale_Internal(ulong entityID, Vector3 scale);
    }
}