using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Iron
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

        internal MaterialPropertyBlock_Internal ToMaterialPropertyBlockInternal()
        {
            MaterialPropertyBlock_Internal properties = new MaterialPropertyBlock_Internal();
            
            properties.floatPropertiesKeys = floatProperties.Keys.ToArray();
            properties.floatPropertiesValues = floatProperties.Values.ToArray();
            
            properties.intPropertiesKeys = intProperties.Keys.ToArray();
            properties.intPropertiesValues = intProperties.Values.ToArray();
            
            properties.texturePropertiesKeys = textureProperties.Keys.ToArray();
            properties.texturePropertiesValues = textureProperties.Values.ToArray();
            
            properties.colorPropertiesKeys = colorProperties.Keys.ToArray();
            properties.colorPropertiesValues = colorProperties.Values.ToArray();

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

            return properties;
        }
    }
}