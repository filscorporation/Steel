using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Loaded aseprite file with parsed sprites and animations
    /// </summary>
    public class AsepriteData : Resource
    {
        internal AsepriteData(uint id) : base(id) { }

        /// <summary>
        /// Sprites parsed from aseprite file
        /// </summary>
        public IEnumerable<Sprite> Sprites
        {
            get
            {
                foreach (uint spriteID in GetSprites_Internal(ID))
                {
                    if (spriteID == NULL_RESOURCE_ID)
                        yield return null;
                    yield return new Sprite(spriteID);
                }
            }
        }

        /// <summary>
        /// Animations parsed from aseprite file
        /// </summary>
        public IEnumerable<Animation> Animations
        {
            get
            {
                foreach (uint animationID in GetAnimations_Internal(ID))
                {
                    if (animationID == NULL_RESOURCE_ID)
                        yield return null;
                    yield return new Animation(animationID);
                }
            }
        }

        /// <summary>
        /// Create entity with sprite and animations from aseprite data
        /// </summary>
        /// <returns>Created entity or null if failed</returns>
        public Entity CreateEntityFromAsepriteData()
        {
            uint entityID = CreateEntityFromAsepriteData_Internal(ID);
            return entityID == Entity.NULL_ENTITY_ID ? null : new Entity(entityID);
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint[] GetSprites_Internal(uint resourceID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint[] GetAnimations_Internal(uint resourceID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateEntityFromAsepriteData_Internal(uint resourceID);
    }
}