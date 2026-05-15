#define _USE_MATH_DEFINES
#include "graphics.h"
#include "world.h"
#include "draw.h"
#include "UI.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
    
float brightness = 1.0f;
extern Room* currentRoomPtr;
extern SimpleModel minoModel;

// --- PARTIKLUK ---
#define MAX_PARTICLES 300
typedef struct {
    float x, y, z, vx, vy, vz, life, homeX, homeZ;
} Particle;

Particle particles[MAX_PARTICLES];

// --- RENDSZER BEÁLLÍTÁSOK ---

void setupCameraProjection(float fovy, float aspect, float zNear, float zFar) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float f = 1.0f / tanf(fovy * M_PI / 360.0f);
    float m[16] = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (zFar+zNear)/(zNear-zFar), -1,
        0, 0, (2.0f*zFar*zNear)/(zNear-zFar), 0
    };
    glMultMatrixf(m);
    glMatrixMode(GL_MODELVIEW);
}

void setupFog() {
    glEnable(GL_FOG);
    GLfloat fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 5.0f);
    glFogf(GL_FOG_END, 25.0f);
}

// --- FÉNYEK ---

void update_lighting() {
    float baseLight = 0.4f * brightness; 
    GLfloat ambient[] = { baseLight, baseLight, baseLight, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    update_lighting();
}

// --- FŐ RAJZOLÓK ---

void drawMap(GameState* state, Assets* assets) {
    glEnable(GL_LIGHTING);
    update_lighting();

    for (int j = 0; j < currentHeight; j++) {
        for (int i = 0; i < currentWidth; i++) {
            float wx = i * 2.0f;
            float wz = j * 2.0f;

            glBindTexture(GL_TEXTURE_2D, assets->floor);
            glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glTexCoord2f(0, 0); glVertex3f(wx - 1, -1.01f, wz - 1);
                glTexCoord2f(1, 0); glVertex3f(wx + 1, -1.01f, wz - 1);
                glTexCoord2f(1, 1); glVertex3f(wx + 1, -1.01f, wz + 1);
                glTexCoord2f(0, 1); glVertex3f(wx - 1, -1.01f, wz + 1);
            glEnd();

            int cell = map[j][i];
            if (cell == 1) {
                glBindTexture(GL_TEXTURE_2D, assets->stone);
                drawWall(wx, wz, 60.0f);
            } else if (cell == 2) {
                drawFire(wx, wz, assets->fire, assets->torch);
            } else if (cell == 3 || cell == 4) {
                glPushMatrix();
                glTranslatef(wx, 0, wz);
                if (i == 0 || i == currentWidth - 1) glRotatef(90, 0, 1, 0);
                drawDoor(0, 0, 3.5f, assets->ajto);
                glTranslatef(0, 3.5f, 0); 
                glBindTexture(GL_TEXTURE_2D, assets->stone);
                drawWall(0, 0, 25.0f);
                glPopMatrix();
            }
            else if (cell == 5 && !state->minoSpawned) { 
                state->minoX = wx; 
                state->minoZ = wz; 
                state->minoSpawned = true; 
            }
            else if (cell == 6) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, assets->sword);
            glEnable(GL_ALPHA_TEST);
            float wx = i * 2.0f;
                float wz = j * 2.0f;

                glPushMatrix();
                glTranslatef(wx, 0.2f, wz);
                glRotatef(SDL_GetTicks() * 0.1f, 0, 1, 0);
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

                glBegin(GL_QUADS);
                    glTexCoord2f(0, 0); glVertex3f(-0.4f, 0.0f, 0.0f);
                    glTexCoord2f(1, 0); glVertex3f( 0.4f, 0.0f, 0.0f);
                    glTexCoord2f(1, 1); glVertex3f( 0.4f, 0.8f, 0.0f);
                    glTexCoord2f(0, 1); glVertex3f(-0.4f, 0.8f, 0.0f);
                glEnd();
                glPopMatrix();
            }
        }
    }
}

// --- PARTICLE ---

void initParticlesEverywhere() {
    if (!currentRoomPtr) return; 
    int count = 0; float coords[200][2];
    for (int j = 0; j < currentRoomPtr->height; j++) {
        for (int i = 0; i < currentRoomPtr->width; i++) {
            if (currentRoomPtr->data[j][i] == 2 && count < 200) {
                coords[count][0] = i * 2.0f; coords[count][1] = j * 2.0f;
                count++;
            }
        }
    }
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (count > 0) {
            int idx = i % count;
            particles[i].homeX = coords[idx][0];
            particles[i].homeZ = coords[idx][1];
            particles[i].life = (float)rand() / RAND_MAX;
        }
    }
}

void updateAndDrawParticles() {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].life <= 0) {
            particles[i].x = particles[i].homeX;
            particles[i].y = 1.2f;
            particles[i].z = particles[i].homeZ;
            particles[i].vx = ((rand() % 100) - 50) / 5000.0f;
            particles[i].vz = ((rand() % 100) - 50) / 5000.0f;
            particles[i].vy = ((rand() % 100) / 1000.0f) + 0.005f;
            particles[i].life = 0.8f + (float)(rand() % 40) / 100.0f;
        }
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
        particles[i].z += particles[i].vz;
        particles[i].vy -= 0.0005f;
        particles[i].life -= 0.01f;
        if (particles[i].life > 0) {
            glColor4f(1.0f, 0.6f, 0.1f, particles[i].life);
            glVertex3f(particles[i].x, particles[i].y, particles[i].z);
        }
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

GLuint loadTexture(const char* filename) {
    SDL_Surface* surf = IMG_Load(filename);
    if (!surf) return 0;
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    GLint mode = (surf->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h, 0, mode, GL_UNSIGNED_BYTE, surf->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surf);
    return tid;
}

void loadOBJ(const char* filename, SimpleModel* model) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("HIBA: Nem talalhato a fajl: %s\n", filename);
        return;
    }

    model->numVertices = 0;
    model->numTexCoords = 0;
    model->numFaces = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ' && model->numVertices < MAX_VERTICES) {
            sscanf(line, "v %f %f %f", 
                &model->vertices[model->numVertices].x, 
                &model->vertices[model->numVertices].y, 
                &model->vertices[model->numVertices].z);
            model->numVertices++;
        } 
        else if (line[0] == 'v' && line[1] == 't' && model->numTexCoords < MAX_VERTICES) {
            sscanf(line, "vt %f %f", 
                &model->texcoords[model->numTexCoords].u, 
                &model->texcoords[model->numTexCoords].v);
            model->numTexCoords++;
        }
        else if (line[0] == 'f' && line[1] == ' ' && model->numFaces < MAX_FACES - 3) {
            int v1, v2, v3;
            int vt1 = 0, vt2 = 0, vt3 = 0; 
            int vn1, vn2, vn3;

            if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) >= 6) {
            } else if (sscanf(line, "f %d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3) >= 3) {
            } else if (sscanf(line, "f %d %d %d", &v1, &v2, &v3) >= 3) {
                vt1 = vt2 = vt3 = 0;
            }

            model->faces[model->numFaces].v = v1 - 1;
            model->faces[model->numFaces].vt = vt1 - 1;
            model->numFaces++;

            model->faces[model->numFaces].v = v2 - 1;
            model->faces[model->numFaces].vt = vt2 - 1;
            model->numFaces++;

            model->faces[model->numFaces].v = v3 - 1;
            model->faces[model->numFaces].vt = vt3 - 1;
            model->numFaces++;
        }
    }
    fclose(file);
}

void drawSimpleModel(SimpleModel* model) {
    if (model->numFaces == 0) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, model->texture);
    
    glEnable(GL_LIGHTING); 
    glColor3f(1.0f, 1.0f, 1.0f); 

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < model->numFaces; i++) {
        int vIdx = model->faces[i].v;
        int tIdx = model->faces[i].vt;
        if (tIdx >= 0) {
            glTexCoord2f(model->texcoords[tIdx].u, 1.0f - model->texcoords[tIdx].v);
        }
        glVertex3f(model->vertices[vIdx].x, model->vertices[vIdx].y, model->vertices[vIdx].z);
    }
    glEnd();
}