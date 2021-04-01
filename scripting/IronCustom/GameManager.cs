using System;
using System.Linq;
using Iron;

namespace IronCustom
{
    public class GameManager
    {
        public static void EntryPoint()
        {
            Screen.Width = 1200;
            Screen.Height = 800;
            Debug.EnableDebugInfoWindow();
        
            //UITest();
            //Test1();
            
            Entity entity = new Entity();
            entity.AddComponent<PhysicsTest>();

            Log.LogInfo("EntryPoint completed");
        }

        private static void UITest()
        {
            Entity entity = new Entity();
            entity.AddComponent<UITestManager>();
            
            Sprite sprite = ResourcesManager.LoadImage("button.png");
            sprite.SetAsSpriteSheet(100, 50);

            const int count = 1;
            for (int i = 0; i < count; i++)
            {
                //CreateUIImage(sprite, (float) (i) / count * 800, (float)i / count * 500);
            }
            for (int i = 0; i < count; i++)
            {
                CreateUIButton(sprite, 600 - (float)(i) / count * 800, 100 + (float)(i) / count * 500, 1.0f + (float)i / count);
            }
            
            CreateUIText("This is some testing text", -200, -200, -1, 48);
            //CreateUIImage(sprite, 0, 200);
            
            //CreateUIText("XD", 200, 350, 1, 120);
            //CreateUIImage(sprite, 200, 350);
        }

        private static void CreateUIText(string text, float x, float y, float z, int size)
        {
            Entity entity = UI.CreateUIElement();
            UIText uiText = entity.AddComponent<UIText>();
            uiText.Text = text;
            uiText.TextSize = size;
            uiText.RectTransform.AnchorMin = new Vector2(0.0f, 0.0f);
            uiText.RectTransform.AnchorMax = new Vector2(1.0f, 1.0f);
            uiText.RectTransform.Size = new Vector2(400, 200);
            uiText.RectTransform.AnchoredPosition = new Vector2(x, y);
            uiText.RectTransform.SortingOrder = z;
        }
        
        private static void CreateUIImage(Sprite sprite, float x, float y)
        {
            Entity entity = UI.CreateUIElement();
            UIImage uiImage = entity.AddComponent<UIImage>();
            uiImage.Sprite = sprite;
            uiImage.RectTransform.AnchorMin = new Vector2(0.5f, 0.0f);
            uiImage.RectTransform.AnchorMax = new Vector2(0.5f, 0.0f);
            uiImage.RectTransform.Size = new Vector2(400, 200);
            uiImage.RectTransform.AnchoredPosition = new Vector2(x, y);
            uiImage.RectTransform.SortingOrder = 0;
            entity.AddComponent<AlphaTest>();
        }

        private static void CreateUIButton(Sprite sprite, float x, float y, float z)
        {
            Entity entity = UI.CreateUIElement();
            UIButton uiButton = entity.AddComponent<UIButton>();
            uiButton.Sprite = sprite;
            uiButton.RectTransform.AnchorMin = new Vector2(0.0f, 0.0f);
            uiButton.RectTransform.AnchorMax = new Vector2(0.0f, 0.0f);
            uiButton.RectTransform.Size = new Vector2(400, 200);
            uiButton.RectTransform.AnchoredPosition = new Vector2(x, y);
            uiButton.RectTransform.SortingOrder = z;
            
            uiButton.Transition = new SpriteTileChangeTransition
            {
                Normal = 0,
                Hovered = 1,
                Clicked = 2,
                Disabled = 3,
                TransitionDuration = 0.8f,
            };

            uiButton.OnClick.AddCallback(() => uiButton.Interactable = false);
            
            Entity entity2 = UI.CreateUIElement();
            UIText uiText = entity2.AddComponent<UIText>();
            uiText.Text = "Button";
            uiText.TextSize = 112;
            uiText.RectTransform.AnchorMin = new Vector2(0.0f, 0.0f);
            uiText.RectTransform.AnchorMax = new Vector2(1.0f, 1.0f);
            uiText.RectTransform.SortingOrder = -1;
            uiText.TextAlignment = AlignmentType.CenterMiddle;
            uiText.Color = new Color(67, 82, 61);
            
            entity2.Parent = entity;
        }

        private static void CallbackTest()
        {
            Log.LogInfo("Clicked on button in CallbackTest");
        }

        private static void Test1()
        {
            Sprite sprite1 = ResourcesManager.LoadImage("yellow-alpha.png");
            Sprite sprite2 = ResourcesManager.LoadImage("green-alpha.png");
            Sprite sprite3 = ResourcesManager.LoadImage("black-alpha.png");
            Sprite sprite4 = ResourcesManager.LoadImage("red-alpha.png");
            Sprite sprite5 = ResourcesManager.LoadImage("blue-alpha.png");

            Sprite sprite6 = ResourcesManager.LoadImage("box.png");
            Sprite sprite7 = ResourcesManager.LoadImage("box_over.png");

            Entity entity = new Entity();
            entity.AddComponent<SpriteRenderer>().Sprite = sprite6;
            entity.AddComponent<BoxCollider>();
            entity.AddComponent<RigidBody>().RigidBodyType = RigidBodyType.Static;
            entity.Transformation.Position = new Vector3(2.0f, 0, 2);
            entity.AddComponent<AlphaTest>();
            
            for (int i = 0; i < 1; i++)
            {
                AddAlphaTest(sprite1, 0 - i * 0.01f, entity);
                AddAlphaTest(sprite2, -2 - i * 0.01f, entity);
                AddAlphaTest(sprite3, 1 - i * 0.01f, entity);
                AddAlphaTest(sprite4, -3 - i * 0.01f, entity);
                AddAlphaTest(sprite5, -1 - i * 0.01f, entity);
                AddAlphaTest(sprite7, 0.5f - i * 0.01f, entity);
            }

            //Camera.Main.Entity.AddComponent<CameraController>();
            
            Log.LogInfo("Test find all sprite renderers");
            Log.LogInfo(string.Join(", ", 
                Component.FindAllOfType<SpriteRenderer>().Select(_SR => _SR.Sprite.ID)));
            
            Log.LogInfo("Test find all circle colliders");
            Log.LogInfo(string.Join(", ", 
                Component.FindAllOfType<CircleCollider>().Select(_CC => _CC.Radius)));
            
            Log.LogInfo("Test find all rigid bodies");
            Log.LogInfo(string.Join(", ", 
                Component.FindAllOfType<RigidBody>().Select(_RB => _RB.RigidBodyType)));

            try
            {
                Log.LogInfo("Test find all alpha tests");
                Log.LogInfo(string.Join(", ", 
                    Component.FindAllOfType<AlphaTest>().Select(_AT => _AT.Entity)));
            }
            catch (Exception e)
            {
                Log.LogWarning(e.Message);
            }
        }

        private static void AddAlphaTest(Sprite sprite, float z, Entity parent)
        {
            Entity entity = new Entity();
            entity.AddComponent<SpriteRenderer>().Sprite = sprite;
            entity.AddComponent<BoxCollider>();
            entity.AddComponent<RigidBody>().RigidBodyType = RigidBodyType.Static;
            entity.Transformation.Position = new Vector3(0, 0, z);
            entity.AddComponent<AlphaTest>();
            //entity.Transformation.Parent = parent.Transformation;
        }
    }
}
