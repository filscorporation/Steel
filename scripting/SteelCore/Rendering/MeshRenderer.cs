using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Renders custom mesh in the scene
    /// </summary>
    public class MeshRenderer : Component
    {
        /// <summary>
        /// Mesh
        /// </summary>
        public Mesh Mesh
        {
            get
            {
                GetMesh_Internal(Entity.ID, out Mesh mesh);
                return mesh;
            }
            set => SetMesh_Internal(Entity.ID, value);
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
        private static extern void GetMesh_Internal(uint entityID, out Mesh mesh);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMesh_Internal(uint entityID, Mesh mesh);

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