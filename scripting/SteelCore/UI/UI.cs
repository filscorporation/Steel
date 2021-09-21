using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Methods to create UI elements and access UI layout properties
    /// </summary>
    public static class UI
    {
        /// <summary>
        /// Creates new UI element in layout
        /// </summary>
        /// <returns>Created entity</returns>
        public static Entity CreateUIElement() => new Entity(CreateUIElement_Internal());
        
        /// <summary>
        /// Creates new UI element in layout
        /// </summary>
        /// <param name="name">Entity name</param>
        /// <returns>Created entity</returns>
        public static Entity CreateUIElement(string name) => new Entity(CreateUIElement_Internal2(name));
        
        /// <summary>
        /// Creates new UI element in layout
        /// </summary>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created entity</returns>
        public static Entity CreateUIElement(string name, Entity parent)
            => new Entity(CreateUIElement_Internal3(name, parent?.ID ?? Entity.NULL_ENTITY_ID));
        
        /// <summary>
        /// Creates new UI image in layout
        /// </summary>
        /// <returns>Created image</returns>
        public static UIImage CreateUIImage()
        {
            Entity entity = new Entity(CreateUIImage_Internal());
            UIImage image = new UIImage();
            image.Entity = entity;

            return image;
        }
        
        /// <summary>
        /// Creates new UI image in layout
        /// </summary>
        /// <param name="sprite">UI image sprite</param>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created image</returns>
        public static UIImage CreateUIImage(Sprite sprite, string name, Entity parent)
        {
            Entity entity = new Entity(
                CreateUIImage_Internal2(sprite?.ID ?? Resource.NULL_RESOURCE_ID, name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UIImage image = new UIImage();
            image.Entity = entity;

            return image;
        }
        
        /// <summary>
        /// Creates new UI button in layout
        /// </summary>
        /// <returns>Created button</returns>
        public static UIButton CreateUIButton()
        {
            Entity entity = new Entity(CreateUIButton_Internal());
            UIButton button = new UIButton();
            button.Entity = entity;

            return button;
        }
        
        /// <summary>
        /// Creates new UI button in layout
        /// </summary>
        /// <param name="sprite">UI button sprite</param>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created button</returns>
        public static UIButton CreateUIButton(Sprite sprite, string name, Entity parent)
        {
            Entity entity = new Entity(
                CreateUIButton_Internal2(sprite?.ID ?? Resource.NULL_RESOURCE_ID, name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UIButton button = new UIButton();
            button.Entity = entity;

            return button;
        }
        
        /// <summary>
        /// Creates new UI text in layout
        /// </summary>
        /// <returns>Created text element</returns>
        public static UIText CreateUIText()
        {
            Entity entity = new Entity(CreateUIText_Internal());
            UIText uiText = new UIText();
            uiText.Entity = entity;

            return uiText;
        }
        
        /// <summary>
        /// Creates new UI text in layout
        /// </summary>
        /// <param name="text">Text value</param>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created text element</returns>
        public static UIText CreateUIText(string text, string name, Entity parent)
        {
            Entity entity = new Entity(CreateUIText_Internal2(text, name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UIText uiText = new UIText();
            uiText.Entity = entity;

            return uiText;
        }
        
        /// <summary>
        /// Creates new UI input field in layout
        /// </summary>
        /// <returns>Created input field</returns>
        public static UIInputField CreateUIInputField()
        {
            Entity entity = new Entity(CreateUIInputField_Internal());
            UIInputField field = new UIInputField();
            field.Entity = entity;

            return field;
        }
        
        /// <summary>
        /// Creates new UI input field in layout
        /// </summary>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created input field</returns>
        public static UIInputField CreateUIInputField(string name, Entity parent)
        {
            Entity entity = new Entity(CreateUIInputField_Internal2(name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UIInputField field = new UIInputField();
            field.Entity = entity;

            return field;
        }
        
        /// <summary>
        /// Creates new UI clipping in layout
        /// </summary>
        /// <returns>Created clipping element</returns>
        public static UIClipping CreateUIClipping()
        {
            Entity entity = new Entity(CreateUIClipping_Internal());
            UIClipping element = new UIClipping();
            element.Entity = entity;

            return element;
        }
        
        /// <summary>
        /// Creates new UI clipping in layout
        /// </summary>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created clipping element</returns>
        public static UIClipping CreateUIClipping(Entity parent)
        {
            Entity entity = new Entity(CreateUIClipping_Internal2(parent?.ID ?? Entity.NULL_ENTITY_ID));
            UIClipping element = new UIClipping();
            element.Entity = entity;

            return element;
        }
        
        /// <summary>
        /// Creates new UI check box in layout
        /// </summary>
        /// <returns>Created element</returns>
        public static UICheckBox CreateUICheckBox()
        {
            Entity entity = new Entity(CreateUICheckBox_Internal());
            UICheckBox element = new UICheckBox();
            element.Entity = entity;

            return element;
        }

        /// <summary>
        /// Creates new UI check box in layout
        /// </summary>
        /// <param name="label">Text to display near element</param>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created element</returns>
        public static UICheckBox CreateUICheckBox(string label, string name, Entity parent)
        {
            Entity entity = new Entity(CreateUICheckBox_Internal2(label, name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UICheckBox element = new UICheckBox();
            element.Entity = entity;

            return element;
        }
        
        /// <summary>
        /// Creates new UI tabs in layout
        /// </summary>
        /// <returns>Created element</returns>
        public static UITabs CreateUITabs()
        {
            Entity entity = new Entity(CreateUITabs_Internal());
            UITabs element = new UITabs();
            element.Entity = entity;

            return element;
        }

        /// <summary>
        /// Creates new UI tabs in layout
        /// </summary>
        /// <param name="tabs">Tabs names</param>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created element</returns>
        public static UITabs CreateUITabs(IEnumerable<string> tabs, string name, Entity parent)
        {
            Entity entity = new Entity(CreateUITabs_Internal2(tabs.ToArray(), name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UITabs element = new UITabs();
            element.Entity = entity;

            return element;
        }
        
        /// <summary>
        /// Creates new UI layout group
        /// </summary>
        /// <returns>Created element</returns>
        public static UILayoutGroup CreateUILayoutGroup(UILayoutGroup.LayoutType type)
        {
            Entity entity = new Entity(CreateUILayoutGroup_Internal(type));
            UILayoutGroup element = new UILayoutGroup();
            element.Entity = entity;

            return element;
        }

        /// <summary>
        /// Creates new UI layout group
        /// </summary>
        /// <param name="type">Layout group type</param>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created element</returns>
        public static UILayoutGroup CreateUILayoutGroup(UILayoutGroup.LayoutType type, string name, Entity parent)
        {
            Entity entity = new Entity(CreateUILayoutGroup_Internal2(type, name, parent?.ID ?? Entity.NULL_ENTITY_ID));
            UILayoutGroup element = new UILayoutGroup();
            element.Entity = entity;

            return element;
        }

        /// <summary>
        /// Is there any UI object under mouse cursor
        /// </summary>
        public static bool IsPointerOverUI() => IsPointerOverUI_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIElement_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIElement_Internal2(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIElement_Internal3(string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIImage_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIImage_Internal2(uint spriteID, string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIButton_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIButton_Internal2(uint spriteID, string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIText_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIText_Internal2(string text, string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIInputField_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIInputField_Internal2(string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIClipping_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIClipping_Internal2(uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUICheckBox_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUICheckBox_Internal2(string label, string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUITabs_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUITabs_Internal2(string[] tabs, string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUILayoutGroup_Internal(UILayoutGroup.LayoutType type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUILayoutGroup_Internal2(UILayoutGroup.LayoutType type, string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsPointerOverUI_Internal();
    }
}