using System.Runtime.CompilerServices;

namespace Steel
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
                ulong spriteID = GetSprite_Internal(Entity.EntityID);
                return spriteID == Resource.NULL_RESOURCE_ID ? null : new Sprite(spriteID);
            }
            set => SetSprite_Internal(Entity.EntityID, value?.ID ?? Resource.NULL_RESOURCE_ID);
        }
        
        /// <summary>
        /// Material
        /// </summary>
        public Material Material
        {
            get => new Material(GetMaterial_Internal(Entity.EntityID));
            set => SetMaterial_Internal(Entity.EntityID, value?.ID ?? Resource.NULL_RESOURCE_ID);
        }
        
        /// <summary>
        /// Color
        /// </summary>
        public Color Color
        {
            get => GetColor_Internal(Entity.EntityID);
            set => SetColor_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Should this element consume UI events
        /// </summary>
        public bool ConsumeEvents
        {
            get => GetConsumeEvents_Internal(Entity.EntityID);
            set => SetConsumeEvents_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Material properties, that applied just for this entity (returned object is copy, and shouldn't be changed)
        /// </summary>
        public MaterialPropertyBlock CustomMaterialProperties
        {
            get
            {
                GetCustomMaterialProperties_Internal(Entity.EntityID, out MaterialPropertyBlock_Internal properties);
                return properties.ToMaterialPropertyBlock();
            }
            set => SetCustomMaterialProperties_Internal(Entity.EntityID, value.ToMaterialPropertyBlockInternal());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetSprite_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(uint entityID, ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetMaterial_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMaterial_Internal(uint entityID, ulong materialID);

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