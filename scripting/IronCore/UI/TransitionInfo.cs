using System;
using System.Runtime.InteropServices;

namespace Iron
{
    /// <summary>
    /// Defines what type of element transition is selected
    /// </summary>
    public enum TransitionType
    {
        ColorShift,
        SpriteChange,
        SpriteTileChange,
        Animation,
    }

    internal struct TransitionData
    {
        public TransitionData(uint value)
        {
            Value = value;
        }
        
        public uint Value;

        public Color ToColor()
        {
            return new Color((Value >> 24) / 255.0f, (Value << 8 >> 24) / 255.0f,
                (Value << 16 >> 24) / 255.0f, (Value << 24 >> 24) / 255.0f);
        }

        public static TransitionData FromColor(Color color)
        {
            return new TransitionData(((uint)(color.R * 255) << 24) + ((uint)(color.G * 255) << 16) 
                  + ((uint)(color.B * 255) << 8) + (uint)(color.A * 255));
        }
    }
    
    /// <summary>
    /// Base class for UI elements transitions
    /// </summary>
    public abstract class TransitionsInfo
    {
        internal TransitionsInfo(TransitionType type)
        {
            TransitionType = type;
        }
        
        public TransitionType TransitionType { get; }
        public float TransitionDuration;

        internal abstract TransitionsInfo_Internal ToInternal();
    }

    /// <summary>
    /// UI element will change sprite color
    /// </summary>
    public class ColorShiftTransition : TransitionsInfo
    {
        public ColorShiftTransition() : base(TransitionType.ColorShift)
        {
            
        }
        
        /// <summary>
        /// Normal element state
        /// </summary>
        public Color Normal;
        
        /// <summary>
        /// Element state when under the mouse cursor
        /// </summary>
        public Color Hovered;
        
        /// <summary>
        /// Selected element state
        /// </summary>
        public Color Selected;
        
        /// <summary>
        /// Element state when it gets clicked
        /// </summary>
        public Color Clicked;
        
        /// <summary>
        /// Element state when it was set as not interactable
        /// </summary>
        public Color Disabled;

        internal override TransitionsInfo_Internal ToInternal()
        {
            return new TransitionsInfo_Internal
            {
                TransitionType = TransitionType.ColorShift,
                TransitionDuration = TransitionDuration,
                Normal = TransitionData.FromColor(Normal),
                Hovered = TransitionData.FromColor(Hovered),
                Selected = TransitionData.FromColor(Selected),
                Clicked = TransitionData.FromColor(Clicked),
                Disabled = TransitionData.FromColor(Disabled),
            };
        }

        internal static ColorShiftTransition FromInternal(TransitionsInfo_Internal info)
        {
            return new ColorShiftTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = info.Normal.ToColor(),
                Hovered = info.Hovered.ToColor(),
                Selected = info.Selected.ToColor(),
                Clicked = info.Clicked.ToColor(),
                Disabled = info.Disabled.ToColor(),
            };
        }
    }
    
    /// <summary>
    /// Element will change sprite
    /// </summary>
    public class SpriteChangeTransition : TransitionsInfo
    {
        public SpriteChangeTransition() : base(TransitionType.SpriteChange)
        {
            
        }
        
        /// <summary>
        /// Normal element state
        /// </summary>
        public Sprite Normal;
        
        /// <summary>
        /// Element state when under the mouse cursor
        /// </summary>
        public Sprite Hovered;
        
        /// <summary>
        /// Selected element state
        /// </summary>
        public Sprite Selected;
        
        /// <summary>
        /// Element state when it gets clicked
        /// </summary>
        public Sprite Clicked;
        
        /// <summary>
        /// Element state when it was set as not interactable
        /// </summary>
        public Sprite Disabled;

        internal override TransitionsInfo_Internal ToInternal()
        {
            return new TransitionsInfo_Internal
            {
                TransitionType = TransitionType.SpriteChange,
                TransitionDuration = TransitionDuration,
                Normal = new TransitionData(Normal.ID),
                Hovered = new TransitionData(Hovered.ID),
                Selected = new TransitionData(Selected.ID),
                Clicked = new TransitionData(Clicked.ID),
                Disabled = new TransitionData(Disabled.ID),
            };
        }

        internal static SpriteChangeTransition FromInternal(TransitionsInfo_Internal info)
        {
            return new SpriteChangeTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = new Sprite(info.Normal.Value),
                Hovered = new Sprite(info.Hovered.Value),
                Selected = new Sprite(info.Selected.Value),
                Clicked = new Sprite(info.Clicked.Value),
                Disabled = new Sprite(info.Disabled.Value),
            };
        }
    }
    
    /// <summary>
    /// Element will change sprite tile index (sprite should be set as sprite sheet)
    /// </summary>
    public class SpriteTileChangeTransition : TransitionsInfo
    {
        public SpriteTileChangeTransition() : base(TransitionType.SpriteTileChange)
        {
            
        }

        /// <summary>
        /// Normal element state
        /// </summary>
        public uint Normal;
        
        /// <summary>
        /// Element state when under the mouse cursor
        /// </summary>
        public uint Hovered;
        
        /// <summary>
        /// Selected element state
        /// </summary>
        public uint Selected;
        
        /// <summary>
        /// Element state when it gets clicked
        /// </summary>
        public uint Clicked;
        
        /// <summary>
        /// Element state when it was set as not interactable
        /// </summary>
        public uint Disabled;

        internal override TransitionsInfo_Internal ToInternal()
        {
            return new TransitionsInfo_Internal
            {
                TransitionType = TransitionType.SpriteTileChange,
                TransitionDuration = TransitionDuration,
                Normal = new TransitionData(Normal),
                Hovered = new TransitionData(Hovered),
                Selected = new TransitionData(Selected),
                Clicked = new TransitionData(Clicked),
                Disabled = new TransitionData(Disabled),
            };
        }

        internal static SpriteTileChangeTransition FromInternal(TransitionsInfo_Internal info)
        {
            return new SpriteTileChangeTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = info.Normal.Value,
                Hovered = info.Hovered.Value,
                Selected = info.Normal.Value,
                Clicked = info.Clicked.Value,
                Disabled = info.Disabled.Value,
            };
        }
    }
    
    /// <summary>
    /// Element will play animation in attached animator
    /// </summary>
    public class AnimationTransition : TransitionsInfo
    {
        public AnimationTransition() : base(TransitionType.Animation)
        {
            
        }
        
        /// <summary>
        /// Normal element state
        /// </summary>
        public Animation Normal;
        
        /// <summary>
        /// Element state when under the mouse cursor
        /// </summary>
        public Animation Hovered;
        
        /// <summary>
        /// Selected element state
        /// </summary>
        public Animation Selected;
        
        /// <summary>
        /// Element state when it gets clicked
        /// </summary>
        public Animation Clicked;
        
        /// <summary>
        /// Element state when it was set as not interactable
        /// </summary>
        public Animation Disabled;

        internal override TransitionsInfo_Internal ToInternal()
        {
            return new TransitionsInfo_Internal
            {
                TransitionType = TransitionType.Animation,
                TransitionDuration = TransitionDuration,
                Normal = new TransitionData(Normal.ID),
                Hovered = new TransitionData(Hovered.ID),
                Selected = new TransitionData(Selected.ID),
                Clicked = new TransitionData(Clicked.ID),
                Disabled = new TransitionData(Disabled.ID),
            };
        }

        internal static AnimationTransition FromInternal(TransitionsInfo_Internal info)
        {
            return new AnimationTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = new Animation(info.Normal.Value),
                Hovered = new Animation(info.Hovered.Value),
                Selected = new Animation(info.Selected.Value),
                Clicked = new Animation(info.Clicked.Value),
                Disabled = new Animation(info.Disabled.Value),
            };
        }
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct TransitionsInfo_Internal
    {
        public TransitionType TransitionType;
        public float TransitionDuration;
        public TransitionData Normal;
        public TransitionData Hovered;
        public TransitionData Selected;
        public TransitionData Clicked;
        public TransitionData Disabled;

        internal TransitionsInfo ToTransitionsInfo()
        {
            switch (TransitionType)
            {
                case TransitionType.ColorShift:
                    return ColorShiftTransition.FromInternal(this);
                case TransitionType.SpriteChange:
                    return SpriteChangeTransition.FromInternal(this);
                case TransitionType.SpriteTileChange:
                    return SpriteTileChangeTransition.FromInternal(this);
                case TransitionType.Animation:
                    return AnimationTransition.FromInternal(this);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}