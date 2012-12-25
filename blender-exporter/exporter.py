import bpy
import ctypes
import os
import subprocess
import fcntl

class Matrix:
    def __init__(self,matrix):
        self.values = []
        for x in range(4):
            for y in range(4):
                self.values.append(matrix[x][y])
    def to_stream(self, stream):
        for val in self.values:
            tmp = ctypes.c_float(val)
            stream.write(bytes(tmp))
        
class Mesh:
    def __init__(self,mesh,matrix):
        self.points = []
        self.triangles = []
        self.add_mesh_data(mesh)
        self.matrix = matrix
        self.material = -1
        
    def add_mesh_data(self,mesh):
        for vert in mesh.vertices:
            self.points.append(vert.co)
        for polygon in mesh.polygons:
            a = (polygon.vertices[0], polygon.vertices[1], polygon.vertices[2])
            self.triangles.append(a)
            if(len(polygon.vertices) == 4):
                a = (polygon.vertices[2], polygon.vertices[3], polygon.vertices[0])
                self.triangles.append(a)
            
    def to_stream(self,stream):
        type = ctypes.c_int(1)
        stream.write(bytes(type))
        self.matrix.to_stream(stream)
        mat = ctypes.c_int(self.material)
        stream.write(bytes(mat))
        num_points = ctypes.c_int(len(self.points))
        stream.write(bytes(num_points))
        for point in self.points:
            for v in point:
                a = ctypes.c_float(v)
                stream.write(bytes(a))
        num_triangles = ctypes.c_int(len(self.triangles))
        stream.write(bytes(num_triangles))
        for triangle in self.triangles:
            for i in triangle:
                a = ctypes.c_int(i)
                stream.write(bytes(a))
                
class PointLight():
    def __init__(self,light,matrix):
        self.matrix = matrix
        self.strength = light.energy
        
    def to_stream(self,stream):
        type = ctypes.c_int(2)
        stream.write(bytes(type))
        self.matrix.to_stream(stream)
        strength = ctypes.c_float(self.strength)
        stream.write(bytes(strength))
      
      
class AreaLight():
    def __init__(self,light,matrix):
        self.matrix = matrix
        self.strength = light.energy
        self.size_x = light.size
        if light.shape == 'SQUARE':
            self.size_y = light.size
        else:
            self.size_y = light.size_y
        
    def to_stream(self,stream):
        type = ctypes.c_int(3)
        stream.write(bytes(type))
        self.matrix.to_stream(stream)
        strength = ctypes.c_float(self.strength)
        stream.write(bytes(strength))
        size = ctypes.c_float(self.size_x)
        stream.write(bytes(size))
        size = ctypes.c_float(self.size_y)
        stream.write(bytes(size))
        
class Material():
    def __init__(self,material):
        self.material = material
        
    def color_to_stream(self,color,stream):
        for i in range(3):
            c = ctypes.c_float(color[i])
            stream.write(bytes(c))
        c = ctypes.c_float(1.0)
        stream.write(bytes(c))
        
    def to_stream(self,stream):
        mat = self.material
        type = ctypes.c_int(0)
        stream.write(bytes(type))
        self.color_to_stream(mat.diffuse_color,stream)
        spec_int = ctypes.c_float(mat.raytracer.reflectivity/100.0)
        stream.write(bytes(spec_int))

class RaytracerRenderEngine(bpy.types.RenderEngine):
        bl_idname = 'RAYTRACER'
        bl_label = "Raytracer"
        def __init__(self):
            self.path_to_executable = "/home/vidarn/code/raytracer/new/raytracer"
            self.res_x = 512
            self.res_y = 512
            pass
        
        def update(self,data,scene):
            self.camera_matrix = bpy.context.scene.camera.matrix_world.copy()
            self.camera_matrix.invert()
            self.objects = []
            self.materials = []
            for obj in bpy.data.objects:
                m = Matrix(self.camera_matrix * obj.matrix_world)
                if obj.type == 'MESH':
                    s = Mesh(obj.data,m)
                    self.objects.append(s)
                    if len(obj.material_slots) > 0:
                        mat = Material(obj.material_slots[0].material)
                        s.material = len(self.materials)
                        self.materials.append(mat)
                    else:
                        print("object " + obj.name + " does not have any material")
                if obj.type == 'LAMP':
                    if obj.data.type == 'POINT':
                        print("point light!")
                        l = PointLight(obj.data,m)
                        self.objects.append(l)
                    if obj.data.type == 'AREA':
                        print("area light!")
                        l = AreaLight(obj.data,m)
                        self.objects.append(l)
                        
        def export_camera(self,scene,stream):
            camera = scene.camera.data
            fov = ctypes.c_float(camera.angle)
            stream.write(bytes(fov))
            focus_dist = ctypes.c_float(camera.dof_distance)
            stream.write(bytes(focus_dist))
            
        def export_render_settings(self,scene,stream):
            res_x = ctypes.c_int(self.res_x)
            stream.write(bytes(res_x))
            res_y = ctypes.c_int(self.res_y)
            stream.write(bytes(res_y))
            threads = ctypes.c_int(scene.raytracer.threads)
            stream.write(bytes(threads))
            
                    
        def render(self,scene):
            self.res_x = int(scene.render.resolution_x * (scene.render.resolution_percentage / 100.0))
            self.res_y = int(scene.render.resolution_y * (scene.render.resolution_percentage / 100.0))
            self.image_path = "/tmp/frame" + str(scene.frame_current).zfill(4) + ".tif"
            stream = open("/tmp/test.scn","wb")
            self.export_render_settings(scene,stream)
            self.export_camera(scene,stream)
            for mat in self.materials:
                mat.to_stream(stream)
            for obj in self.objects:
                obj.to_stream(stream)
            stream.close()
            print("--------- done exporting ------------")
            args = (self.path_to_executable,self.image_path)
            process = subprocess.Popen(args, stdout = None)
            while not os.path.isfile(self.image_path):
                pass
            last_image_update = os.stat(self.image_path).st_mtime
            while process.poll() == None:
                image_update = os.stat(self.image_path).st_mtime
                if image_update != last_image_update:
                    self.update_image()
                    last_image_update = image_update
            self.update_image()
            
        def update_image(self):
            result =self.begin_result(0,0,self.res_x,self.res_y)
            lay = result.layers[0]
            f = open(self.image_path)
            fcntl.lockf(f,fcntl.LOCK_SH)
            lay.load_from_file(self.image_path)
            fcntl.lockf(f,fcntl.LOCK_UN)
            f.close()
            self.end_result(result)
                
        def __del__(self):
            pass
        
  
from bl_ui import properties_data_camera
for member in dir(properties_data_camera):
    subclass = getattr(properties_data_camera, member)
    try:
        subclass.COMPAT_ENGINES.add('RAYTRACER')
    except:
        pass
del properties_data_camera

from bl_ui import properties_material
properties_material.MATERIAL_PT_context_material.COMPAT_ENGINES.add('RAYTRACER')
from bl_ui import properties_data_lamp
properties_data_lamp.DATA_PT_lamp.COMPAT_ENGINES.add('RAYTRACER')
properties_data_lamp.DATA_PT_area.COMPAT_ENGINES.add('RAYTRACER')
from bl_ui import properties_render
properties_render.RENDER_PT_render.COMPAT_ENGINES.add('RAYTRACER')
properties_render.RENDER_PT_dimensions.COMPAT_ENGINES.add('RAYTRACER')


del properties_material

class MaterialButtonsPanel():
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "material"
    # COMPAT_ENGINES must be defined in each subclass, external engines can add themselves here

    @classmethod
    def poll(cls, context):
        mat = context.material
        rd = context.scene.render
        return mat and (rd.use_game_engine is False) and (rd.engine in cls.COMPAT_ENGINES)
    
class MATERIAL_PT_raytracer_diffuse(MaterialButtonsPanel, bpy.types.Panel):
    bl_label = "Diffuse"
    COMPAT_ENGINES = {'RAYTRACER'}

    def draw_header(self, context):
        scene = context.material

        #self.layout.prop(scene.pov, "mirror_use_IOR", text="")

    def draw(self, context):
        layout = self.layout

        mat = context.material
        layout.active = True

        col = layout.column()
        col.alignment = 'CENTER'
        col.prop(mat, "diffuse_color")
        
        
class MATERIAL_PT_raytracer_reflection(MaterialButtonsPanel, bpy.types.Panel):
    bl_label = "Reflection"
    COMPAT_ENGINES = {'RAYTRACER'}

    def draw_header(self, context):
        scene = context.material

        #self.layout.prop(scene.pov, "mirror_use_IOR", text="")

    def draw(self, context):
        layout = self.layout

        mat = context.material
        layout.active = True

        col = layout.column()
        col.alignment = 'CENTER'
        col.prop(mat.raytracer, "reflectivity")
        
        
class RenderButtonsPanel():
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "render"
    # COMPAT_ENGINES must be defined in each subclass, external engines can add themselves here

    @classmethod
    def poll(cls, context):
        rd = context.scene.render
        return (rd.use_game_engine == False) and (rd.engine in cls.COMPAT_ENGINES)

class RENDER_PT_raytracer_threads(RenderButtonsPanel, bpy.types.Panel):
    bl_label = "Threads"
    COMPAT_ENGINES = {'RAYTRACER'}

    def draw_header(self, context):
        scene = context.material

        #self.layout.prop(scene.pov, "mirror_use_IOR", text="")

    def draw(self, context):
        layout = self.layout

        scene = context.scene
        layout.active = True

        col = layout.column()
        col.alignment = 'CENTER'
        col.prop(scene.raytracer, "threads")
        
class RaytracerSettingsMaterial(bpy.types.PropertyGroup):
    reflectivity = bpy.props.FloatProperty(
            name="Reflectivity",
            description="Reflectivity of the material",
            subtype="PERCENTAGE",
            default=0.0,
            min=0.0,
            max=100.0)

class RaytracerSettingsScene(bpy.types.PropertyGroup):
    threads = bpy.props.IntProperty(
            name="Threads",
            description="Number of renderer threads",
            subtype="UNSIGNED",
            default=2,
            min=1,
            max=200)
        
def register():
    bpy.utils.register_module(__name__)
    bpy.types.Material.raytracer = bpy.props.PointerProperty(type=RaytracerSettingsMaterial)
    bpy.types.Scene.raytracer = bpy.props.PointerProperty(type=RaytracerSettingsScene)
 
if __name__ == "__main__":       
    bpy.utils.unregister_module(__name__)
    register()