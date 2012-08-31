
function mingedInit()
   atlas = minged_atlas_create()
   img = minged_atlas_add(atlas, "minged/test.png")
   minged_atlas_generate(atlas)
   minged_image_destroy(img)
end

function mingedUpdate()
end

function mingedRender()
end

function mingedCleanup()
   minged_atlas_destroy(atlas)
end