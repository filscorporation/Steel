using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// UI element that will get rendered in the UI layout
    /// </summary>
    public class UIImage : UIComponent
    {
        /// <summary>
        /// Sprite to render
        /// </summary>
        public Sprite Sprite
        {
            get
            {
                uint spriteID = GetSprite_Internal(Entity.ID);
                return spriteID == Resource.NULL_RESOURCE_ID ? null : new Sprite(spriteID);
            }
            set => SetSprite_Internal(Entity.ID, value?.ID ?? Resource.NULL_RESOURCE_ID);
        }
        
        /// <summary>
        /// Material
        /// </summary>
        public Material Material
        {
            get => new Material(GetMaterial_Internal(Entity.ID));
            set => SetMaterial_Internal(Entity.ID, value?.ID ?? Resource.NULL_RESOURCE_ID);
        }
        
        /// <summary>
        /// Color
        /// </summary>
        public Color Color
        {
            get => GetColor_Internal(Entity.ID);
            set => SetColor_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Should this element consume UI events
        /// </summary>
        public bool ConsumeEvents
        {
            get => GetConsumeEvents_Internal(Entity.ID);
            set => SetConsumeEvents_Internal(Entity.ID, value);
        }
        
        /// <summary>
        /// Material properties, that applied just for this entity (returned object is copy, and shouldn't be changed)
        /// </summary>
        public MaterialPropertyBlock CustomMaterialProperties
        {
            get
            {
                GetCustomMaterialProperties_Internal(Entity.ID, out MaterialPropertyBlock_Internal properties);
                return properties.ToMaterialPropertyBlock();
            }
            set => SetCustomMaterialProperties_Internal(Entity.ID, value.ToMaterialPropertyBlockInternal());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetSprite_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(uint entityID, uint spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetMaterial_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMaterial_Internal(uint entityID, uint materialID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Color GetColor_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetColor_Internal(uint entityID, Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetConsumeEvents_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetConsumeEvents_Internal(uint entityID, bool consume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCustomMaterialProperties_Internal(uint entityID, out MaterialPropertyBlock_Internal properties);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCustomMaterialProperties_Internal(uint entityID, MaterialPropertyBlock_Internal properties);
    }
}