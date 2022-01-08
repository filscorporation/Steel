using Steel;

namespace SteelCustom
{
    public class GameManager
    {
        public static void EntryPoint()
        {
            /*
            Entry point will be called after Application initialized and before main loop will start.
            Here you can initialize game objects, add components and other game related stuff.
            This is temporary solution before editor and scene serialization will be added to engine.
            Some code examples:

            //PrecisionTest();
            //MultilineTextTest();
            //InputFieldTest();
            //MultilineInputTextTest();
            //ClippingTest();
            //AlphaBlendingTest();
            //CheckBoxTest();
            //TabsTest();
            //LayoutTest();
            //UIAnimationsTest();
            //UIEventsTest();

            Entity entity = new Entity();
            //entity.AddComponent<Benchmark>();
            entity.AddComponent<PhysicsTest>();
            //entity.AddComponent<ClippingTest>();
            //entity.AddComponent<MeshTest>();
            
            Log.LogInfo("UUID " + entity.UUID);

            Log.LogInfo("EntryPoint completed");
        }
    }
}