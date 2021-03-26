using System;
using System.Runtime.InteropServices;

namespace Iron
{
    public enum ButtonTransitionType
    {
        ColorShift,
        SpriteChange,
        SpriteTileChange,
        Animation,
    }

    internal struct ButtonTransitionData
    {
        public ButtonTransitionData(uint value)
        {
            Value = value;
        }
        
        public uint Value;

        public Color ToColor()
        {
            return new Color((Value >> 24) / 255.0f, (Value << 8 >> 24) / 255.0f,
                (Value << 16 >> 24) / 255.0f, (Value << 24 >> 24) / 255.0f);
        }

        public static ButtonTransitionData FromColor(Color color)
        {
            return new ButtonTransitionData(((uint)(color.R * 255) << 24) + ((uint)(color.G * 255) << 16) 
                  + ((uint)(color.B * 255) << 8) + (uint)(color.A * 255));
        }
    }
    
    public abstract class ButtonTransitionsInfo
    {
        internal ButtonTransitionsInfo(ButtonTransitionType type)
        {
            TransitionType = type;
        }
        
        public ButtonTransitionType TransitionType { get; }
        public float TransitionDuration;

        internal abstract ButtonTransitionsInfo_Internal ToInternal();
    }

    public class ColorShiftTransition : ButtonTransitionsInfo
    {
        public ColorShiftTransition() : base(ButtonTransitionType.ColorShift)
        {
            
        }
        
        public Color Normal;
        public Color Hovered;
        public Color Clicked;
        public Color Disabled;

        internal override ButtonTransitionsInfo_Internal ToInternal()
        {
            return new ButtonTransitionsInfo_Internal
            {
                TransitionType = ButtonTransitionType.ColorShift,
                TransitionDuration = TransitionDuration,
                Normal = ButtonTransitionData.FromColor(Normal),
                Hovered = ButtonTransitionData.FromColor(Hovered),
                Clicked = ButtonTransitionData.FromColor(Clicked),
                Disabled = ButtonTransitionData.FromColor(Disabled),
            };
        }

        internal static ColorShiftTransition FromInternal(ButtonTransitionsInfo_Internal info)
        {
            return new ColorShiftTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = info.Normal.ToColor(),
                Hovered = info.Hovered.ToColor(),
                Clicked = info.Clicked.ToColor(),
                Disabled = info.Disabled.ToColor(),
            };
        }
    }

    public class SpriteChangeTransition : ButtonTransitionsInfo
    {
        public SpriteChangeTransition() : base(ButtonTransitionType.SpriteChange)
        {
            
        }
        
        public Sprite Normal;
        public Sprite Hovered;
        public Sprite Clicked;
        public Sprite Disabled;

        internal override ButtonTransitionsInfo_Internal ToInternal()
        {
            return new ButtonTransitionsInfo_Internal
            {
                TransitionType = ButtonTransitionType.SpriteChange,
                TransitionDuration = TransitionDuration,
                Normal = new ButtonTransitionData(Normal.ID),
                Hovered = new ButtonTransitionData(Hovered.ID),
                Clicked = new ButtonTransitionData(Clicked.ID),
                Disabled = new ButtonTransitionData(Disabled.ID),
            };
        }

        internal static SpriteChangeTransition FromInternal(ButtonTransitionsInfo_Internal info)
        {
            return new SpriteChangeTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = new Sprite(info.Normal.Value),
                Hovered = new Sprite(info.Normal.Value),
                Clicked = new Sprite(info.Normal.Value),
                Disabled = new Sprite(info.Normal.Value),
            };
        }
    }

    public class SpriteTileChangeTransition : ButtonTransitionsInfo
    {
        public SpriteTileChangeTransition() : base(ButtonTransitionType.SpriteTileChange)
        {
            
        }
        
        public uint Normal;
        public uint Hovered;
        public uint Clicked;
        public uint Disabled;

        internal override ButtonTransitionsInfo_Internal ToInternal()
        {
            return new ButtonTransitionsInfo_Internal
            {
                TransitionType = ButtonTransitionType.SpriteTileChange,
                TransitionDuration = TransitionDuration,
                Normal = new ButtonTransitionData(Normal),
                Hovered = new ButtonTransitionData(Hovered),
                Clicked = new ButtonTransitionData(Clicked),
                Disabled = new ButtonTransitionData(Disabled),
            };
        }

        internal static SpriteTileChangeTransition FromInternal(ButtonTransitionsInfo_Internal info)
        {
            return new SpriteTileChangeTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = info.Normal.Value,
                Hovered = info.Normal.Value,
                Clicked = info.Normal.Value,
                Disabled = info.Normal.Value,
            };
        }
    }

    public class AnimationTransition : ButtonTransitionsInfo
    {
        public AnimationTransition() : base(ButtonTransitionType.Animation)
        {
            
        }
        
        public Animation Normal;
        public Animation Hovered;
        public Animation Clicked;
        public Animation Disabled;

        internal override ButtonTransitionsInfo_Internal ToInternal()
        {
            return new ButtonTransitionsInfo_Internal
            {
                TransitionType = ButtonTransitionType.Animation,
                TransitionDuration = TransitionDuration,
                Normal = new ButtonTransitionData(Normal.ID),
                Hovered = new ButtonTransitionData(Hovered.ID),
                Clicked = new ButtonTransitionData(Clicked.ID),
                Disabled = new ButtonTransitionData(Disabled.ID),
            };
        }

        internal static AnimationTransition FromInternal(ButtonTransitionsInfo_Internal info)
        {
            return new AnimationTransition
            {
                TransitionDuration = info.TransitionDuration,
                Normal = new Animation(info.Normal.Value),
                Hovered = new Animation(info.Normal.Value),
                Clicked = new Animation(info.Normal.Value),
                Disabled = new Animation(info.Normal.Value),
            };
        }
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct ButtonTransitionsInfo_Internal
    {
        public ButtonTransitionType TransitionType;
        public float TransitionDuration;
        public ButtonTransitionData Normal;
        public ButtonTransitionData Hovered;
        public ButtonTransitionData Clicked;
        public ButtonTransitionData Disabled;

        internal ButtonTransitionsInfo ToButtonTransitionsInfo()
        {
            switch (TransitionType)
            {
                case ButtonTransitionType.ColorShift:
                    return ColorShiftTransition.FromInternal(this);
                case ButtonTransitionType.SpriteChange:
                    return SpriteChangeTransition.FromInternal(this);
                case ButtonTransitionType.SpriteTileChange:
                    return SpriteTileChangeTransition.FromInternal(this);
                case ButtonTransitionType.Animation:
                    return AnimationTransition.FromInternal(this);
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}