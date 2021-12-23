using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Renders sprite (image) in the scene
    /// </summary>
    public class SpriteRenderer : Component
    {
        /// <summary>
        /// Sprite to render
        /// </summary>
        public Sprite Sprite
        {
            get => new Sprite(GetSprite_Internal(Entity.EntityID));
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
        /// Material properties, that applied just for this object (returned object is copy, and shouldn't be changed)
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
        private static extern uint GetSprite_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(uint entityID, uint spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetMaterial_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMaterial_Internal(uint entityID, uint materialID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetCustomMaterialProperties_Internal(uint entityID, out MaterialPropertyBlock_Internal properties);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCustomMaterialProperties_Internal(uint entityID, MaterialPropertyBlock_Internal properties);
    }
}