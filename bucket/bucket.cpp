#include "bucket.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene, int startPixel[2], int endPixel[2], int width):m_viewPlane(viewPlane), m_scene(scene), m_width(width)
{
    for (int i = 0; i < 2; i++) {
        m_startPixel[i] = startPixel[i];
        m_endPixel[i] = endPixel[i];
    }
    m_done = false;
    m_outlineSize = 5;
    m_numMinSamples = 3;
    m_numMaxSamples = 5;
    m_sampler = new RandomSampler();
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::render()
{
    int pos[2];
    for(int corner = 0; corner < 4; corner++){
        for(int axis = 0; axis < 2; axis++){
            for(int a=0; a< m_outlineSize; a++){
                pos[axis] = a;
                pos[1-axis] = 0;
                if(corner < 2){
                    pos[0] += m_startPixel[0];
                }
                else{
                    pos[0] = m_endPixel[0] - pos[0]-1;
                }
                if(corner % 2){
                    pos[1] += m_startPixel[1];
                }
                else{
                    pos[1] = m_endPixel[1] - pos[1]-1;
                }
                RGBA pixel(1.0f,1.0f,1.0f,1.0f);
                int i = pos[0]+pos[1]*m_width;
                m_viewPlane->setPixelValue(i,pixel);
            }
        }
    }
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            int pixelId = x+y*m_width;
            int numSamples = m_numMinSamples;
			std::vector<RGBA> samples;
			sample(numSamples, pixelId, samples);
			if(getVariance(samples) > 0.002f){
				numSamples = m_numMaxSamples - m_numMinSamples;
				sample(numSamples, pixelId, samples);
			}
            RGBA color;
            float invNumSamples = 1.0/(float)samples.size();
			for(int i=0; i<samples.size(); i++){
				color += samples[i]*invNumSamples;
			}
			if(getVariance(samples) > 0.002f){
				//color = RGBA(1.0f,1.0f,1.0f,1.0f);
			}
            m_pixels.push_back(color);
        }
    }
    int a = 0;
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            int i = x+y*m_width;
            m_viewPlane->setPixelValue(i,m_pixels[a]);
            a++;
        }
    }
    m_done = true;
}

void Bucket::sample(int numSamples, int pixelId, std::vector<RGBA> &samples)
{
	for(int i=0; i<numSamples; i++){
		Vec3 tmp_sample = m_sampler->getSquareSample();
		Ray ray = m_viewPlane->getPixelRay(pixelId,tmp_sample);
		samples.push_back(m_scene->trace(ray));
	}
}

float Bucket::getVariance(std::vector<RGBA> &samples)
{
	float mean = 0;
	for( int i = 0; i < samples.size(); i++){
		mean += samples[i].value();
	}
	mean = mean/samples.size();
	float accum = 0;
	for( int i = 0; i < samples.size(); i++){
		float tmp = samples[i].value() - mean;
		accum += tmp*tmp;
	}
	accum = accum/(samples.size()-1);
	accum = sqrtf(accum);
	//std::cout << "accum: " << accum << std::endl;
	return accum;
}
