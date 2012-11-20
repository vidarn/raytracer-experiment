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
            for obj in bpy.data.objects:
                m = Matrix(self.camera_matrix * obj.matrix_world)
                if obj.type == 'MESH':
                    s = Mesh(obj.data,m)
                    self.objects.append(s)
                if obj.type == 'LAMP':
                    print("light!")
                    l = PointLight(obj.data,m)
                    self.objects.append(l)
        def render(self,scene):
            self.image_path = "/tmp/out.tif"
            stream = open("/tmp/test.scn","wb")
            for obj in self.objects:
                obj.to_stream(stream)
            stream.close()
            process = subprocess.Popen(self.path_to_executable, stdout = None)
            last_image_update = os.stat(self.image_path).st_mtime_ns
            while process.poll() == None:
                image_update = os.stat(self.image_path).st_mtime_ns
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