using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Components that will get rendered in the scene
    /// </summary>
    public class SpriteRenderer : Component
    {
        /// <summary>
        /// Sprite to render
        /// </summary>
        public Sprite Sprite
        {
            get => new Sprite(GetSprite_Internal(Entity.ID));
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
        /// Material properties, that applied just for this object (returned object is copy, and shouldn't be changed)
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
        private static extern void GetCustomMaterialProperties_Internal(uint entityID, out MaterialPropertyBlock_Internal properties);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCustomMaterialProperties_Internal(uint entityID, MaterialPropertyBlock_Internal properties);
    }
}