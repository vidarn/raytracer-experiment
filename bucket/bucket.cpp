#include "bucket.h"
#include "../sampler/sampling.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene, unsigned int id):m_viewPlane(viewPlane), m_scene(scene)
{
	m_id = id;
    m_sampler = new RandomSampler(134134+42343*id);
	m_cameraPath = new PathNode[scene->m_settings.m_cameraBounces];
	m_lightPath  = new PathNode[scene->m_settings.m_lightBounces ];
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::render(int startX, int startY)
{
	int x = startX/2;
	int y = startY/2;
	int pass = 0;
    while(true){
		while(y<m_viewPlane->m_resolution[1]/2){
			while(x<m_viewPlane->m_resolution[0]/2){
				sample(x*2 + pass/2,y*2 + pass%2);
				x++;
			}
			x = 0;
			y++;
		}
		y = 0;
		pass = (pass+1)%4;
    }
}

void Bucket::buildPath(PathNode* &path, Ray &ray, Sampling &sampling, int &numNodes,const RGBA &startColor, int start, int end, int samplingId)
{
    float pdf;
    RGBA col;
    RGBA nextCol = startColor;
	for(int i=start;i<end;i++){
		PathNode &node = path[i];
        node.m_sr.m_hit = false;
        node.m_sr.m_hitT = FLT_MAX;
        node.m_sr.m_depth = i + samplingId;
        m_scene->trace(ray,&node.m_sr);
		if(node.m_sr.m_hit){
			node.m_incident = ray.m_dir.getNormalized();
			node.m_incident.invert();
			node.m_sr.m_material->shade(node.m_sr,m_scene, sampling, &ray, &col, &pdf);
			node.m_accumColor = nextCol;
			nextCol *= col;
			numNodes++;
		}
		else{
			return;
		}
	}
}

void Bucket::sample(int x, int y)
{
	int lightBounces = m_scene->m_settings.m_lightBounces;
	int cameraBounces = m_scene->m_settings.m_cameraBounces;
    Sampling sampling(1,1,2,
                      1,1 + lightBounces * cameraBounces,m_sampler);
    Ray ray = m_viewPlane->getPixelRay(x,y,sampling);
    RGBA finalCol;
    RGBA col;
	int numCameraNodes = 0;
	int numLightNodes = 1;

	buildPath(m_cameraPath,ray,sampling,numCameraNodes, RGBA(1.0f,1.0f,1.0f,1.0f), 0, cameraBounces,1);

	int lightIndex = floor(sampling.get1DSample(0)[0]*m_scene->m_lights.size());
	Light *light = m_scene->m_lights[lightIndex];
	ray = light->getRay(sampling);
	m_lightPath[0].m_sr.m_hitPos = ray.m_origin;
	m_lightPath[0].m_accumColor = light->getLightColor(ray.m_origin);
	buildPath(m_lightPath ,ray,sampling,numLightNodes, m_lightPath[0].m_accumColor, 1, lightBounces,1 + cameraBounces);

	for(int j=0;j<numCameraNodes;j++){
		PathNode &cameraNode = m_cameraPath[j];
		for(int i=0;i<numLightNodes;i++){
			PathNode &lightNode  =  m_lightPath[i];
			Vec3 dir = lightNode.m_sr.m_hitPos - cameraNode.m_sr.m_hitPos;
			dir.normalize();

			float delta = 0.0001f;
			Vec3 tmp = dir * delta;
			tmp = cameraNode.m_sr.m_hitPos + tmp;
			Ray ray = Ray(tmp,dir,true);
			ray.computePlucker();

			float shadeVal = m_scene->traceShadow(ray);
			shadeVal *= float(m_scene->m_lights.size());

			col = lightNode.m_accumColor * cameraNode.m_accumColor * shadeVal;
			cameraNode.m_sr.m_material->shade(cameraNode.m_incident, dir, cameraNode.m_sr, &col, 1.0f);
			finalCol += col;
		}
	}
    finalCol[3] = 1.0f;

    m_viewPlane->setPixelValue(x,y,finalCol);
}
