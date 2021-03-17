using System.Runtime.CompilerServices;

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

        public Transformation Parent
        {
            get
            {
                uint parentEntityID = GetParent_Internal(Entity.ID);
                if (parentEntityID == Entity.NULL_ENTITY_ID)
                    return null;
                Transformation parent = new Transformation();
                parent.Entity = new Entity(parentEntityID);

                return parent;
            }
            set => SetParent_Internal(Entity.ID, value == null ? Entity.NULL_ENTITY_ID : value.Entity.ID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetPosition_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPosition_Internal(uint entityID, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetRotation_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRotation_Internal(uint entityID, Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetScale_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetScale_Internal(uint entityID, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetParent_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetParent_Internal(uint entityID, uint parentEntityID);
    }
}