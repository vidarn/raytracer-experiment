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
        print("export material")
        type = ctypes.c_int(0)
        stream.write(bytes(type))
        self.color_to_stream(self.material.diffuse_color,stream)
        #name = ctypes.create_string_buffer(self.material.name)
#        stream.write(bytes(type))

class RaytracerRenderEngine(bpy.types.RenderEngine):
        bl_idname = 'TEST'
        bl_label = "Raytracer"
        def __init__(self):
            self.path_to_executable = "/home/vidar/code/raytracer/raytracer"
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
                    print("light!")
                    l = PointLight(obj.data,m)
                    self.objects.append(l)
                    
        def export_camera(self,scene,stream):
            camera = scene.camera.data
            fov = ctypes.c_float(camera.angle)
            stream.write(bytes(fov))
            focus_dist = ctypes.c_float(camera.dof_distance)
            stream.write(bytes(focus_dist))
                    
        def render(self,scene):
            self.image_path = "/tmp/frame" + str(scene.frame_current).zfill(4) + ".tif"
            stream = open("/tmp/test.scn","wb")
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
            result =self.begin_result(0,0,512,512)
            lay = result.layers[0]
            f = open(self.image_path)
            fcntl.lockf(f,fcntl.LOCK_SH)
            lay.load_from_file(self.image_path)
            fcntl.lockf(f,fcntl.LOCK_UN)
            f.close()
            self.end_result(result)
                
        def __del__(self):
            pass
 
if __name__ == "__main__":       
    bpy.utils.unregister_module(__name__)
    bpy.utils.register_module(__name__)