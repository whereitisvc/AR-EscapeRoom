/*
 *  ARWrapperNativeCarsExample.cpp
 *  ARToolKit for Android
 *
 *  Disclaimer: IMPORTANT:  This Daqri software is supplied to you by Daqri
 *  LLC ("Daqri") in consideration of your agreement to the following
 *  terms, and your use, installation, modification or redistribution of
 *  this Daqri software constitutes acceptance of these terms.  If you do
 *  not agree with these terms, please do not use, install, modify or
 *  redistribute this Daqri software.
 *
 *  In consideration of your agreement to abide by the following terms, and
 *  subject to these terms, Daqri grants you a personal, non-exclusive
 *  license, under Daqri's copyrights in this original Daqri software (the
 *  "Daqri Software"), to use, reproduce, modify and redistribute the Daqri
 *  Software, with or without modifications, in source and/or binary forms;
 *  provided that if you redistribute the Daqri Software in its entirety and
 *  without modifications, you must retain this notice and the following
 *  text and disclaimers in all such redistributions of the Daqri Software.
 *  Neither the name, trademarks, service marks or logos of Daqri LLC may
 *  be used to endorse or promote products derived from the Daqri Software
 *  without specific prior written permission from Daqri.  Except as
 *  expressly stated in this notice, no other rights or licenses, express or
 *  implied, are granted by Daqri herein, including but not limited to any
 *  patent rights that may be infringed by your derivative works or by other
 *  works in which the Daqri Software may be incorporated.
 *
 *  The Daqri Software is provided by Daqri on an "AS IS" basis.  DAQRI
 *  MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 *  THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE DAQRI SOFTWARE OR ITS USE AND
 *  OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *  IN NO EVENT SHALL DAQRI BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 *  MODIFICATION AND/OR DISTRIBUTION OF THE DAQRI SOFTWARE, HOWEVER CAUSED
 *  AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 *  STRICT LIABILITY OR OTHERWISE, EVEN IF DAQRI HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Copyright 2015 Daqri LLC. All Rights Reserved.
 *  Copyright 2011-2015 ARToolworks, Inc. All Rights Reserved.
 *
 *  Author(s): Julian Looser, Philip Lamb
 */

#include <AR/gsub_es.h>
#include <Eden/glm.h>
#include <jni.h>
#include <ARWrapper/ARToolKitWrapperExportedAPI.h>
#include <unistd.h> // chdir()
#include <android/log.h>

// Utility preprocessor directive so only one change needed if Java class name changes
#define JNIFUNCTION_DEMO(sig) Java_org_artoolkit_ar_samples_ARSimpleNativeCars_SimpleNativeRenderer_##sig

extern "C" {
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoInitialise(JNIEnv * env, jobject object)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoShutdown(JNIEnv * env, jobject object)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceCreated(JNIEnv * env, jobject object)) ;
JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceChanged(JNIEnv * env, jobject object, jint w, jint h)) ;
JNIEXPORT int JNICALL
JNIFUNCTION_DEMO(demoDrawFrame(JNIEnv * env, jobject obj, jint num, jint snum)) ;
};

typedef struct ARModel {
    int patternID;
    ARdouble transformationMatrix[16];
    bool visible;
    GLMmodel *obj;
} ARModel;

#define NUM_MODELS 6
static ARModel models[NUM_MODELS] = {0};

static float lightAmbient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
static float lightDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static float lightPosition[4] = {0.0f, 0.0f, 1.0f, 0.0f};

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoInitialise(JNIEnv * env, jobject object)) {

    const char *model0file = "Data/models/sword.obj"; //sword.obj
    const char *model1file = "Data/models/high.obj"; //(key)
    const char *model2file = "Data/models/Porsche_911_GT3.obj"; //

    const char *model3file = "Data/models/toon_boy.obj"; //zombie.obj
    const char *model4file = "Data/models/chain.obj";
    const char *model5file = "Data/models/budhha.obj"; //budhha.obj

    /******************Model 0********************************************************/
    models[0].patternID = arwAddMarker("single;Data/hiro.patt;80");
    arwSetMarkerOptionBool(models[0].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
    arwSetMarkerOptionBool(models[0].patternID, ARW_MARKER_OPTION_FILTERED, true);

    models[0].obj = glmReadOBJ2(model0file, 0, 0); // context 0, don't read textures yet.
    if (!models[0].obj) {
    LOGE("Error loading model from file '%s'.", model0file);
    exit(-1);
    }
    glmScale(models[0].obj, 25.00f);
    //glmRotate(models[0].obj, 3.14159f / 2.0f, 1.0f, 0.0f, 0.0f);
    glmCreateArrays(models[0].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    models[0].visible = false;


    /******************Model 1********************************************************/
    models[1].patternID = arwAddMarker("single;Data/kanji.patt;80");
    arwSetMarkerOptionBool(models[1].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
    arwSetMarkerOptionBool(models[1].patternID, ARW_MARKER_OPTION_FILTERED, true);

    models[1].obj = glmReadOBJ2(model1file, 0, 0); // context 0, don't read textures yet.
    if (!models[1].obj) {
    LOGE("Error loading model from file '%s'.", model1file);
    exit(-1);
    }
    glmScale(models[1].obj, 1500.00f);
    //glmRotate(models[1].obj, 3.14159f / 2.0f, 1.0f, 0.0f, 0.0f);
    glmCreateArrays(models[1].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    models[1].visible = false;


    /******************Model 2********************************************************/
    models[2].patternID = arwAddMarker("single;Data/marker16.patt;80");
    arwSetMarkerOptionBool(models[2].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
    arwSetMarkerOptionBool(models[2].patternID, ARW_MARKER_OPTION_FILTERED, true);

    models[2].obj = glmReadOBJ2(model2file, 0, 0); // context 0, don't read textures yet.
    if (!models[2].obj) {
    LOGE("Error loading model from file '%s'.", model2file);
    exit(-1);
    }
    glmScale(models[2].obj, 0.05f);
    //glmRotate(models[2].obj, 3.14159f / 2.0f, 1.0f, 0.0f, 0.0f);
    glmCreateArrays(models[2].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    models[2].visible = false;


    /******************Model 3********************************************************/
    models[3].patternID = arwAddMarker("single;Data/marker17.patt;80");
    arwSetMarkerOptionBool(models[3].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
    arwSetMarkerOptionBool(models[3].patternID, ARW_MARKER_OPTION_FILTERED, true);

    models[3].obj = glmReadOBJ2(model3file, 0, 0); // context 0, don't read textures yet.
    if (!models[3].obj) {
    LOGE("Error loading model from file '%s'.", model3file);
    exit(-1);
    }
    glmScale(models[3].obj, 75.00f);
    glmRotate(models[3].obj, 3.14159f / 2.0f, 1.0f, 0.0f, 0.0f);
    glmCreateArrays(models[3].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    models[3].visible = false;


    /******************Model 4********************************************************/
    models[4].patternID = arwAddMarker("single;Data/marker18.patt;80");
    arwSetMarkerOptionBool(models[4].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
    arwSetMarkerOptionBool(models[4].patternID, ARW_MARKER_OPTION_FILTERED, true);

    models[4].obj = glmReadOBJ2(model4file, 0, 0); // context 0, don't read textures yet.
    if (!models[4].obj) {
    LOGE("Error loading model from file '%s'.", model4file);
    exit(-1);
    }
    glmScale(models[4].obj, 0.50f);
    //glmRotate(models[1].obj, 3.14159f / 2.0f, 1.0f, 0.0f, 0.0f);
    glmCreateArrays(models[4].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    models[4].visible = false;


    /******************Model 5********************************************************/
    models[5].patternID = arwAddMarker("single;Data/marker19.patt;80");
    arwSetMarkerOptionBool(models[5].patternID, ARW_MARKER_OPTION_SQUARE_USE_CONT_POSE_ESTIMATION, false);
    arwSetMarkerOptionBool(models[5].patternID, ARW_MARKER_OPTION_FILTERED, true);

    models[5].obj = glmReadOBJ2(model5file, 0, 0); // context 0, don't read textures yet.
    if (!models[5].obj) {
    LOGE("Error loading model from file '%s'.", model4file);
    exit(-1);
    }
    glmScale(models[5].obj, 0.5f);
    glmRotate(models[5].obj, 3.14159f / 2.0f, 1.0f, 0.0f, 0.0f);
    glmCreateArrays(models[5].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
    models[5].visible = false;

}

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoShutdown(JNIEnv * env, jobject object)) {
}

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceCreated(JNIEnv * env, jobject object)) {
    glStateCacheFlush(); // Make sure we don't hold outdated OpenGL state.
    for (int i = 0;i < NUM_MODELS; i++) {
    if (models[i].obj) {
    glmDelete(models[i].obj, 0);
    models[i].obj = NULL;
    }
    }
}

JNIEXPORT void JNICALL
JNIFUNCTION_DEMO(demoSurfaceChanged(JNIEnv * env, jobject object, jint w, jint h)) {
// glViewport(0, 0, w, h) has already been set.
}

JNIEXPORT int JNICALL
JNIFUNCTION_DEMO(demoDrawFrame(JNIEnv * env, jobject obj, jint clicknum, jint solvenum)) {

    //LOGE("clicknum = %d",clicknum);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the projection matrix to that provided by ARToolKit.
    float proj[16];
    arwGetProjectionMatrix(proj);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(proj);
    glMatrixMode(GL_MODELVIEW);

    glStateCacheEnableDepthTest();

    glStateCacheEnableLighting();

    glEnable(GL_LIGHT0);


    int MarkerID = -1;
    for (int i = 0;i < NUM_MODELS; i++) {
        models[i].visible = arwQueryMarkerTransformation(models[i].patternID, models[i].transformationMatrix);

        if (models[i].visible) {
        glLoadMatrixf(models[i].transformationMatrix);

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        if(clicknum != i && i<3 ){  // first 3 models are "keys"
            glmRotate(models[i].obj, 3.14159f / (2.0f*25.0f), 0.0f, 0.0f, 1.0f);
            glmCreateArrays(models[i].obj, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
            glmDrawArrays(models[i].obj, 0);
        }

        if(solvenum != i && i>=3 ){ // last 3 models are "puzzle"
            glmDrawArrays(models[i].obj, 0);
        }


        MarkerID = i;
        break;
    }
    else
    MarkerID = -1; //detect nothing

    }
    //LOGE("MarkerID = %d", MarkerID);


    return MarkerID;
}
