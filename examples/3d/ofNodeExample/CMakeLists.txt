define_example(ofNodeExample)
target_link_libraries(ofNodeExample of::ofxGui)

copy_example_resource(ofNodeExample bin/data/engine.wav)
copy_example_resource(ofNodeExample bin/data/horn.mp3)
copy_example_resource(ofNodeExample bin/data/reverse.mp3)
copy_example_resource(ofNodeExample bin/data/oflogo.ply)
copy_example_resource(ofNodeExample bin/data/truck.ply)
copy_example_resource(ofNodeExample bin/data/wheel.ply)