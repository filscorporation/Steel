using System;
using Iron;

namespace IronCustom
{
    public class GameManager
    {
        public static void EntryPoint()
        {
            /*

            Entry point will be called after Application initialized and before main loop will start.
            Here you can initialize game objects, add components and other game related stuff.
            This is temporary solution before editor and scene serialization will be added to engine.add

            Some code examples:

            Create entity:
            Entity entity = new Entity();
            Add sprite renderer
            entity.AddComponent<SpriteRenderer>();
            Set it's sprite
            Sprite sprite = ResourcesManager.LoadImage("../resources/my_image.png");
            entity.GetComponent<SpriteRenderer>().Sprite = sprite;
            Move object
            entity.Transformation.Position += new Vector3(0, Time.DeltaTime, 0);

            Your custom components can be created and added to entities:

            public class MyComponent : ScriptComponent
            {
                public void override OnCreate()
                {
                    // Will be called when component is attached to entity
                }

                public void override OnUpdate()
                {
                    // Will be called once per frame
                }
            }

            */

            Log.LogInfo("EntryPoint completed");
        }
    }
}
