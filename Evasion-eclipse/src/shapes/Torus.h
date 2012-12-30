/*
 * MultiMediaTechnology / QPT1 / Öhlinger Bernhard
 * Added Texture coordinates.
 */

#ifndef TORUS_H_
#define TORUS_H_

#include <GL/gl.h>	// GLUT header

/*******************************************************************************
 * Copyright (c) 2000, 2005 IBM Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     IBM Corporation - initial API and implementation
 *******************************************************************************/


/** Added Texture Coordinates to a Torus */

void drawTorus(float r, float R, int nsides, int rings) {
    float ringDelta = 2.0f * (float) M_PI / rings;
    float sideDelta = 2.0f * (float) M_PI / nsides;
    float theta = 0.0f, cosTheta = 1.0f, sinTheta = 0.0f;
    for (int i = rings - 1; i >= 0; i--) {
      float theta1 = theta + ringDelta;
      float cosTheta1 = (float) cos(theta1);
      float sinTheta1 = (float) sin(theta1);
      glBegin(GL_QUAD_STRIP);
      float phi = 0.0f;
      for (int j = nsides; j >= 0; j--) {
        phi += sideDelta;
        float cosPhi = (float) cos(phi);
        float sinPhi = (float) sin(phi);
        float dist = R + r * cosPhi;
        glTexCoord2f(0.0, 0.0);
        glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
        glTexCoord2f(i / rings, j / nsides);
        glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
        glTexCoord2f(i / rings, j / nsides);
        glVertex3f(cosTheta * dist, -sinTheta * dist, r * sinPhi);
      }
      glEnd();
      theta = theta1;
      cosTheta = cosTheta1;
      sinTheta = sinTheta1;
    }
}


#endif /* TORUS_H_ */
