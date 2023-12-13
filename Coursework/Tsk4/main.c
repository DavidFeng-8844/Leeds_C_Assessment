#define P_P_DIFF    1000 /* Threshold value for the difference between maximum and minimum values of 3D data */
#define RISING_EDGE 1    /* Rising edge state */
#define FALLING_EDGE 0   /* Falling edge state */
#define FAST_WALK_TIME_LIMIT_MS  200  /* ms */
#define SLOW_WALK_TIME_LIMIT_MS 10000 /* 10s - time without a valid step to reset step count */
#define STEP_OK 8          /* Number of consecutive valid steps */

unsigned int lastPos = 0;        /* Previous position */
unsigned int newMax = 0, newMin = 0; /* Maximum and minimum values */

bool walkSta = FALSE;    /* State for detecting the first step */
bool walkOkSta = FALSE;  /* State for detecting 7 valid steps within 10s */
bool pSta = RISING_EDGE; /* 3D data state */

long lastTime = 0;          /* Time of the last walkSta transition */
unsigned char stepOK = 0;   /* Initial step counter - reset after an invalid step */
unsigned long stepCount = 0; /* Total step count */

/*****************************************************************
** input: 3 axis or angle
** output: step count
** user read:
	3 axis is filter value.
******************************************************************/
unsigned long Step_Count(float axis0, float axis1, float axis2) {
    unsigned int nowPos = 0;
    int ppDiff = 0;
    int timeDiff = 0;
    /* Get 3D IMU position */
    nowPos = (unsigned int)powf(sqrtf(axis0) + sqrtf(axis1) + sqrtf(axis2), 0.5);
    /* Detect maximum and minimum values */
    if ((pSta == RISING_EDGE) && (nowPos <= lastPos)) {
        pSta = FALLING_EDGE;
        newMax = lastPos;
        walkSta = TRUE;
    } else if ((pSta == FALLING_EDGE) && (nowPos > lastPos)) {
        pSta = FALLING_EDGE;
        newMin = lastPos;
        walkSta = TRUE;
    } else {
        walkSta = FALSE;
    }
    /* Update 3D step state */
    lastPos = nowPos;

    /* Process step detection */
    if (walkSta == TRUE) {
        walkSta = FALSE;
        ppDiff = newMax - newMin; /* Calculate the difference between maximum and minimum values */
        if (ppDiff > P_P_DIFF) {
            timeDiff = GetTime() - lastTime; /* Get time since the last valid step */
            if (timeDiff < FAST_WALK_TIME_LIMIT_MS) { /* If time is less than 200ms, ignore it */
                return stepCount;
            } else if (timeDiff > SLOW_WALK_TIME_LIMIT_MS) { /* If time is greater than 10s, reset step count */
                walkOkSta = FALSE;
                stepOK = 0;
                return stepCount;
            }
            stepOK++;
            if (stepOK >= STEP_OK) { /* If 7 consecutive valid steps are detected */
                walkOkSta = TRUE;
            }
            lastTime = GetTime(); /* Update the time */
        }
    }

    if (walkOkSta == TRUE) { /* If 7 valid steps are detected within 10s */
        stepCount += stepOK;
        stepOK = 0;
    }
    return stepCount;
}
