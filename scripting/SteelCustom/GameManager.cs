using Steel;

namespace SteelCustom
{
    // Script component example
    public class GameManager : ScriptComponent
    {
        public override void OnCreate()
        {
            /*
            This will get called when component is added to the entity
            
            Some code examples:
            -Create entity:
            Entity entity = new Entity();
            -Add sprite renderer
            entity.AddComponent<SpriteRenderer>();
            -Set it's sprite
            Sprite sprite = ResourcesManager.GetImage("my_image.png");
            entity.GetComponent<SpriteRenderer>().Sprite = sprite;
            -Move object
            entity.Transformation.Position += new Vector3(0, Time.DeltaTime, 0);
            -Your custom components can be created and added to entities:
            public class MyComponent : ScriptComponent
            {
                // override engine methods here
            }
            */

            Log.LogInfo("GameManager created");
        }

        public override void OnUpdate()
        {
            /*
            This will get called once per frame
            */
        }
    }
}