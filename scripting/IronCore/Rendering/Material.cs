using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Material
    /// </summary>
    public class Material : Resource
    {
        internal Material(uint id) : base(id) { }
        
        /// <summary>
        /// Shader
        /// </summary>
        public Shader Shader
        {
            get => new Shader(GetShader_Internal(ID));
            set => SetShader_Internal(ID, value?.ID ?? Resource.NULL_RESOURCE_ID);
        }
        
        /// <summary>
        /// Material properties (returned object is copy, and shouldn't be changed)
        /// </summary>
        public MaterialPropertyBlock Properties
        {
            get
            {
                GetProperties_Internal(ID, out MaterialPropertyBlock_Internal properties);
                return properties.ToMaterialPropertyBlock();
            }
            set => SetProperties_Internal(ID, value.ToMaterialPropertyBlockInternal());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetShader_Internal(uint resourceID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetShader_Internal(uint resourceID, uint shaderID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetProperties_Internal(uint resourceID, out MaterialPropertyBlock_Internal properties);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetProperties_Internal(uint resourceID, MaterialPropertyBlock_Internal properties);
    }
}