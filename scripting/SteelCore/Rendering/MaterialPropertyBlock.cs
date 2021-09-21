using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Steel
{
    /// <summary>
    /// Shader properties (uniforms) info that will be applied to rendered entity
    /// </summary>
    public struct MaterialPropertyBlock
    {
        internal Dictionary<string, float> floatProperties;
        internal Dictionary<string, int> intProperties;
        internal Dictionary<string, uint> textureProperties;
        internal Dictionary<string, Color> colorProperties;
        
        internal bool depthMask;
        internal ComparisonFunction depthFunction;
        internal short stencilMask;
        internal ComparisonFunction stencilFunction;
        internal short stencilFunctionRef;
        internal short stencilFunctionMask;
        internal StencilOperation stencilFailOperation;
        internal StencilOperation stencilZFailOperation;
        internal StencilOperation stencilZPassOperation;

        internal bool valuesInitialized;
        
        /// <summary>
        /// Set float uniform
        /// </summary>
        public void SetFloat(string key, float value)
        {
            if (floatProperties == null)
                floatProperties = new Dictionary<string, float>();
            
            floatProperties[key] = value;
        }

        /// <summary>
        /// Get float uniform
        /// </summary>
        public float GetFloat(string key)
        {
            if (floatProperties == null || !floatProperties.TryGetValue(key, out float value))
                throw new KeyNotFoundException($"Key {key} does not exist");
            
            return value;
        }
        
        /// <summary>
        /// Set int uniform
        /// </summary>
        public void SetInt(string key, int value)
        {
            if (intProperties == null)
                intProperties = new Dictionary<string, int>();
            
            intProperties[key] = value;
        }

        /// <summary>
        /// Set int uniform
        /// </summary>
        public float GetInt(string key)
        {
            if (intProperties == null || !intProperties.TryGetValue(key, out int value))
                throw new KeyNotFoundException($"Key {key} does not exist");
            
            return value;
        }
        
        /// <summary>
        /// Set texture (sampler2D) uniform
        /// </summary>
        public void SetTexture(string key, uint textureID)
        {
            if (textureProperties == null)
                textureProperties = new Dictionary<string, uint>();
            
            textureProperties[key] = textureID;
        }
        
        /// <summary>
        /// Get texture (sampler2D) uniform
        /// </summary>
        public uint GetTexture(string key)
        {
            if (textureProperties == null || !textureProperties.TryGetValue(key, out uint value))
                throw new KeyNotFoundException($"Key {key} does not exist");
            
            return value;
        }
        
        /// <summary>
        /// Set color (vec4) uniform
        /// </summary>
        public void SetColor(string key, Color value)
        {
            if (colorProperties == null)
                colorProperties = new Dictionary<string, Color>();
            
            colorProperties[key] = value;
        }

        /// <summary>
        /// Get color (vec4) uniform
        /// </summary>
        public Color GetColor(string key)
        {
            if (colorProperties == null || !colorProperties.TryGetValue(key, out Color value))
                throw new KeyNotFoundException($"Key {key} does not exist");
            
            return value;
        }

        /// <summary>
        /// Enable or disable writing to the depth buffer
        /// </summary>
        /// <param name="mask">Mask. Default is true</param>
        public void SetDepthMask(bool mask)
        {
            if (!valuesInitialized)
                SetDefaultValues();

            depthMask = mask;
        }

        /// <summary>
        /// Sets depth test comparison function
        /// </summary>
        /// <param name="func">Function used to compare with reference value. Default is <see cref="ComparisonFunction.Less"/></param>
        public void SetDepthFunc(ComparisonFunction func)
        {
            if (!valuesInitialized)
                SetDefaultValues();

            depthFunction = func;
        }

        /// <summary>
        /// Sets stencil test parameters
        /// </summary>
        /// <param name="func">Function used to compare with reference value. Default is <see cref="ComparisonFunction.Always"/></param>
        /// <param name="refValue">Reference value. Default is all 0s</param>
        /// <param name="mask">Mask that is ANDed with both the reference value and the stored stencil value when the test is done. Default is all 1s</param>
        public void SetStencilFunc(ComparisonFunction func, short refValue, short mask)
        {
            if (!valuesInitialized)
                SetDefaultValues();

            stencilFunction = func;
            stencilFunctionRef = refValue;
            stencilFunctionMask = mask;
        }

        /// <summary>
        /// Set a bit mask to enable and disable writing of individual bits in the stencil buffer
        /// </summary>
        /// <param name="mask">Mask. Default is all 1s</param>
        public void SetStencilMask(short mask)
        {
            if (!valuesInitialized)
                SetDefaultValues();

            stencilMask = mask;
        }

        /// <summary>
        /// Set stencil test actions
        /// </summary>
        /// <param name="fail">Action to take when the stencil test fails. Default is <see cref="StencilOperation.Keep"/></param>
        /// <param name="zFail">Action when the stencil test passes, but the depth test fails. Default is <see cref="StencilOperation.Keep"/></param>
        /// <param name="zPass">Action when both the stencil test and the depth test pass. Default is <see cref="StencilOperation.Keep"/></param>
        public void SetStencilOperation(StencilOperation fail, StencilOperation zFail, StencilOperation zPass)
        {
            if (!valuesInitialized)
                SetDefaultValues();

            stencilFailOperation = fail;
            stencilZFailOperation = zFail;
            stencilZPassOperation = zPass;
        }

        private void SetDefaultValues()
        {
            valuesInitialized = true;

            depthMask = true;
            depthFunction = ComparisonFunction.Less;
            
            stencilMask = 255;
            stencilFunction = ComparisonFunction.Always;
            stencilFunctionRef = 0;
            stencilFunctionMask = 255;
            stencilFailOperation = StencilOperation.Keep;
            stencilZFailOperation = StencilOperation.Keep;
            stencilZPassOperation = StencilOperation.Keep;
        }

        internal MaterialPropertyBlock_Internal ToMaterialPropertyBlockInternal()
        {
            MaterialPropertyBlock_Internal properties = new MaterialPropertyBlock_Internal();
            
            if (!valuesInitialized)
                SetDefaultValues();
            
            properties.floatPropertiesKeys = floatProperties.Keys.ToArray();
            properties.floatPropertiesValues = floatProperties.Values.ToArray();
            
            properties.intPropertiesKeys = intProperties.Keys.ToArray();
            properties.intPropertiesValues = intProperties.Values.ToArray();
            
            properties.texturePropertiesKeys = textureProperties.Keys.ToArray();
            properties.texturePropertiesValues = textureProperties.Values.ToArray();
            
            properties.colorPropertiesKeys = colorProperties.Keys.ToArray();
            properties.colorPropertiesValues = colorProperties.Values.ToArray();
            
            properties.depthMask = depthMask;
            properties.depthFunction = depthFunction;
            properties.stencilMask = stencilMask;
            properties.stencilFunction = stencilFunction;
            properties.stencilFunctionRef = stencilFunctionRef;
            properties.stencilFunctionMask = stencilFunctionMask;
            properties.stencilFailOperation = stencilFailOperation;
            properties.stencilZFailOperation = stencilZFailOperation;
            properties.stencilZPassOperation = stencilZPassOperation;

            return properties;
        }
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct MaterialPropertyBlock_Internal
    {
        internal string[] floatPropertiesKeys;
        internal float[] floatPropertiesValues;
        internal string[] intPropertiesKeys;
        internal int[] intPropertiesValues;
        internal string[] texturePropertiesKeys;
        internal uint[] texturePropertiesValues;
        internal string[] colorPropertiesKeys;
        internal Color[] colorPropertiesValues;
        
        internal bool depthMask;
        internal ComparisonFunction depthFunction;
        internal short stencilMask;
        internal ComparisonFunction stencilFunction;
        internal short stencilFunctionRef;
        internal short stencilFunctionMask;
        internal StencilOperation stencilFailOperation;
        internal StencilOperation stencilZFailOperation;
        internal StencilOperation stencilZPassOperation;

        internal MaterialPropertyBlock ToMaterialPropertyBlock()
        {
            MaterialPropertyBlock properties = new MaterialPropertyBlock();
            
            properties.floatProperties = new Dictionary<string, float>();
            for (int i = 0; i < floatPropertiesKeys.Length; i++)
                properties.floatProperties[floatPropertiesKeys[i]] = floatPropertiesValues[i];
            
            properties.intProperties = new Dictionary<string, int>();
            for (int i = 0; i < intPropertiesKeys.Length; i++)
                properties.intProperties[intPropertiesKeys[i]] = intPropertiesValues[i];
            
            properties.textureProperties = new Dictionary<string, uint>();
            for (int i = 0; i < texturePropertiesKeys.Length; i++)
                properties.textureProperties[texturePropertiesKeys[i]] = texturePropertiesValues[i];
            
            properties.colorProperties = new Dictionary<string, Color>();
            for (int i = 0; i < colorPropertiesKeys.Length; i++)
                properties.colorProperties[colorPropertiesKeys[i]] = colorPropertiesValues[i];
            
            properties.depthMask = depthMask;
            properties.depthFunction = depthFunction;
            properties.stencilMask = stencilMask;
            properties.stencilFunction = stencilFunction;
            properties.stencilFunctionRef = stencilFunctionRef;
            properties.stencilFunctionMask = stencilFunctionMask;
            properties.stencilFailOperation = stencilFailOperation;
            properties.stencilZFailOperation = stencilZFailOperation;
            properties.stencilZPassOperation = stencilZPassOperation;
            properties.valuesInitialized = true;

            return properties;
        }
    }
}