#include "bucket.h"

Bucket::Bucket(ViewPlane *viewPlane, Scene *scene, int startPixel[2], int endPixel[2], int width):m_viewPlane(viewPlane), m_scene(scene), m_width(width)
{
    for (int i = 0; i < 2; i++) {
        m_startPixel[i] = startPixel[i];
        m_endPixel[i] = endPixel[i];
    }
    m_done = false;
    m_borderDrawn = false;
    m_outlineSize = 5;
    m_numMinSamples = 8;
    m_numMaxSamples = 512;
    m_sampler = new RandomSampler();
    m_numPixels = (endPixel[0] - startPixel[0]) * (endPixel[1] - startPixel[1]);
}

Bucket::~Bucket()
{
    delete m_sampler;
}

void Bucket::drawBorder()
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
    m_borderDrawn = true;
}

void Bucket::render()
{
    drawBorder();
    bool done = false;
    std::vector<RGBA> samples[m_numPixels];
    bool debug[m_numPixels];
    int a=0;
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            int pixelId = x+y*m_width;
            int numSamples = m_numMinSamples;
            sample(numSamples, pixelId, samples[a]);
            debug[a] = false;
            a++;
        }
    }
    while(!done){
        done = true;
        a = 0;
        int width = (m_endPixel[0] - m_startPixel[0]);
        int height= (m_endPixel[1] - m_startPixel[1]);
        for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
            for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
				bool refine = false;
                std::vector<RGBA*> checkSamples;
                if(checkSamples.size() < 4){
                    for(int yoffs = -1; yoffs <=1; yoffs ++){
                        for(int xoffs = -1; xoffs <=1; xoffs ++){
                            for(int i=0;i<samples[a].size();i++){
                                int pixelY = a/width + yoffs;
                                int pixelX = a%width + xoffs;
                                if(pixelX > 0 && pixelX < width){
                                    if(pixelY > 0 && pixelY < height){
                                        int pixelId = pixelX +  pixelY*(m_endPixel[0] - m_startPixel[0]);
                                        checkSamples.push_back(&(samples[pixelId][i]));
                                    }
                                }
                            }
                        }
                    }
                }
                if(getVariance(checkSamples) > 0.01f){
					refine = true;
                }
				if(refine){
                    int pixelId = x+y*m_width;
                    int numSamples = m_numMaxSamples - m_numMinSamples;
                    sample(numSamples, pixelId, samples[a]);
                    //debug[a] = true;
				}
                a++;
            }
        }
    }
    a = 0;
    for(int y=m_startPixel[1]; y<m_endPixel[1]; y++){
        for(int x=m_startPixel[0]; x<m_endPixel[0]; x++){
            RGBA color;
            float invNumSamples = 1.0/(float)samples[a].size();
            for(int i=0; i<samples[a].size(); i++){
                color += samples[a][i]*invNumSamples;
            }
            int i = x+y*m_width;
            if(debug[a]){
                color[2] = 1.0f;
            }
            m_viewPlane->setPixelValue(i,color);
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

float Bucket::getVariance(std::vector<RGBA*> &samples)
{
	float mean = 0;
	for( int i = 0; i < samples.size(); i++){
		mean += samples[i]->value();
	}
	mean = mean/samples.size();
	float accum = 0;
	for( int i = 0; i < samples.size(); i++){
		float tmp = samples[i]->value() - mean;
		accum += tmp*tmp;
	}
	accum = accum/(samples.size()-1);
	accum = sqrtf(accum);
	return accum;
}
