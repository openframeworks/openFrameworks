/*
     File:       SoundSprocket.h
 
     Contains:   SoundSprocket interfaces
 
     Version:    Technology: 1.7
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1996-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __SOUNDSPROCKET__
#define __SOUNDSPROCKET__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __QD3D__
#include <QD3D.h>
#endif

#ifndef __QD3DCAMERA__
#include <QD3DCamera.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif




#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/*******************************************************************************
 *  SndSetInfo/SndGetInfo Messages
 ******************************************************************************/
/*  The siSSpCPULoadLimit = '3dll' selector for SndGetInfo fills in a value of  */
/*  type UInt32.                                                                */

enum {
    kSSpSpeakerKind_Stereo      = 0,
    kSSpSpeakerKind_Mono        = 1,
    kSSpSpeakerKind_Headphones  = 2
};


/*  This is the data type is used with the SndGet/SetInfo selector              */
/*  siSSpSpeakerSetup = '3dst'                                                  */

struct SSpSpeakerSetupData {
    UInt32                          speakerKind;                /* Speaker configuration          */
    float                           speakerAngle;               /* Angle formed by user and speakers  */

    UInt32                          reserved0;                  /* Reserved for future use -- set to 0    */
    UInt32                          reserved1;                  /* Reserved for future use -- set to 0    */
};
typedef struct SSpSpeakerSetupData      SSpSpeakerSetupData;
enum {
    kSSpMedium_Air              = 0,
    kSSpMedium_Water            = 1
};


enum {
    kSSpSourceMode_Unfiltered   = 0,                            /* No filtering applied             */
    kSSpSourceMode_Localized    = 1,                            /* Localized by source position      */
    kSSpSourceMode_Ambient      = 2,                            /* Coming from all around          */
    kSSpSourceMode_Binaural     = 3                             /* Already binaurally localized      */
};



struct SSpLocationData {
    float                           elevation;                  /* Angle of the meridian -- pos is up */
    float                           azimuth;                    /* Angle of the parallel -- pos is left   */
    float                           distance;                   /* Distance between source and listener   */
    float                           projectionAngle;            /* Cos(angle) between cone and listener   */
    float                           sourceVelocity;             /* Speed of source toward the listener    */
    float                           listenerVelocity;           /*Speed of listener toward the source */
};
typedef struct SSpLocationData          SSpLocationData;

struct SSpVirtualSourceData {
    float                           attenuation;                /* Attenuation factor           */
    SSpLocationData                 location;                   /* Location of virtual source        */
};
typedef struct SSpVirtualSourceData     SSpVirtualSourceData;
/*  This is the data type is used with the SndGet/SetInfo selector              */
/*  siSSpLocalization = '3dif'                                                  */

struct SSpLocalizationData {
    UInt32                          cpuLoad;                    /* CPU load vs. quality -- 0 is best  */

    UInt32                          medium;                     /* Medium for sound propagation      */
    float                           humidity;                   /* Humidity when medium is air         */
    float                           roomSize;                   /* Reverb model -- distance bet. walls    */
    float                           roomReflectivity;           /*Reverb model -- bounce attenuation  */
    float                           reverbAttenuation;          /*Reverb model -- mix level      */

    UInt32                          sourceMode;                 /* Type of filtering to apply        */
    float                           referenceDistance;          /*Nominal distance for recording   */
    float                           coneAngleCos;               /* Cos(angle/2) of attenuation cone     */
    float                           coneAttenuation;            /* Attenuation outside the cone      */
    SSpLocationData                 currentLocation;            /* Location of the sound           */

    UInt32                          reserved0;                  /* Reserved for future use -- set to 0    */
    UInt32                          reserved1;                  /* Reserved for future use -- set to 0    */
    UInt32                          reserved2;                  /* Reserved for future use -- set to 0    */
    UInt32                          reserved3;                  /* Reserved for future use -- set to 0    */

    UInt32                          virtualSourceCount;         /*Number of reflections          */
    SSpVirtualSourceData            virtualSource[4];           /*The reflections               */
};
typedef struct SSpLocalizationData      SSpLocalizationData;
#if TARGET_CPU_PPC
typedef CALLBACK_API_C( Boolean , SSpEventProcPtr )(EventRecord *inEvent);
/*******************************************************************************
 *  Global functions
 ******************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
SSpConfigureSpeakerSetup        (SSpEventProcPtr        inEventProcPtr);

EXTERN_API_C( OSStatus )
SSpGetCPULoadLimit              (UInt32 *               outCPULoadLimit);


/*******************************************************************************
 *  Routines for Maniulating Listeners
 ******************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */

typedef struct OpaqueSSpListenerReference*  SSpListenerReference;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
SSpListener_New                 (SSpListenerReference * outListenerReference);

EXTERN_API_C( OSStatus )
SSpListener_Dispose             (SSpListenerReference   inListenerReference);

EXTERN_API_C( OSStatus )
SSpListener_SetTransform        (SSpListenerReference   inListenerReference,
                                 const TQ3Matrix4x4 *   inTransform);

EXTERN_API_C( OSStatus )
SSpListener_GetTransform        (SSpListenerReference   inListenerReference,
                                 TQ3Matrix4x4 *         outTransform);

EXTERN_API_C( OSStatus )
SSpListener_SetPosition         (SSpListenerReference   inListenerReference,
                                 const TQ3Point3D *     inPosition);

EXTERN_API_C( OSStatus )
SSpListener_GetPosition         (SSpListenerReference   inListenerReference,
                                 TQ3Point3D *           outPosition);

EXTERN_API_C( OSStatus )
SSpListener_SetOrientation      (SSpListenerReference   inListenerReference,
                                 const TQ3Vector3D *    inOrientation);

EXTERN_API_C( OSStatus )
SSpListener_GetOrientation      (SSpListenerReference   inListenerReference,
                                 TQ3Vector3D *          outOrientation);

EXTERN_API_C( OSStatus )
SSpListener_SetUpVector         (SSpListenerReference   inListenerReference,
                                 const TQ3Vector3D *    inUpVector);

EXTERN_API_C( OSStatus )
SSpListener_GetUpVector         (SSpListenerReference   inListenerReference,
                                 TQ3Vector3D *          outUpVector);

EXTERN_API_C( OSStatus )
SSpListener_SetCameraPlacement  (SSpListenerReference   inListenerReference,
                                 const TQ3CameraPlacement * inCameraPlacement);

EXTERN_API_C( OSStatus )
SSpListener_GetCameraPlacement  (SSpListenerReference   inListenerReference,
                                 TQ3CameraPlacement *   outCameraPlacement);

EXTERN_API_C( OSStatus )
SSpListener_SetVelocity         (SSpListenerReference   inListenerReference,
                                 const TQ3Vector3D *    inVelocity);

EXTERN_API_C( OSStatus )
SSpListener_GetVelocity         (SSpListenerReference   inListenerReference,
                                 TQ3Vector3D *          outVelocity);

EXTERN_API_C( OSStatus )
SSpListener_GetActualVelocity   (SSpListenerReference   inListenerReference,
                                 TQ3Vector3D *          outVelocity);

EXTERN_API_C( OSStatus )
SSpListener_SetMedium           (SSpListenerReference   inListenerReference,
                                 UInt32                 inMedium,
                                 float                  inHumidity);

EXTERN_API_C( OSStatus )
SSpListener_GetMedium           (SSpListenerReference   inListenerReference,
                                 UInt32 *               outMedium,
                                 float *                outHumidity);

EXTERN_API_C( OSStatus )
SSpListener_SetReverb           (SSpListenerReference   inListenerReference,
                                 float                  inRoomSize,
                                 float                  inRoomReflectivity,
                                 float                  inReverbAttenuation);

EXTERN_API_C( OSStatus )
SSpListener_GetReverb           (SSpListenerReference   inListenerReference,
                                 float *                outRoomSize,
                                 float *                outRoomReflectivity,
                                 float *                outReverbAttenuation);

EXTERN_API_C( OSStatus )
SSpListener_SetMetersPerUnit    (SSpListenerReference   inListenerReference,
                                 float                  inMetersPerUnit);

EXTERN_API_C( OSStatus )
SSpListener_GetMetersPerUnit    (SSpListenerReference   inListenerReference,
                                 float *                outMetersPerUnit);


/*******************************************************************************
 *  Routines for Manipulating Sources
 ******************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */

typedef struct OpaqueSSpSourceReference*  SSpSourceReference;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( OSStatus )
SSpSource_New                   (SSpSourceReference *   outSourceReference);

EXTERN_API_C( OSStatus )
SSpSource_Dispose               (SSpSourceReference     inSourceReference);

EXTERN_API_C( OSStatus )
SSpSource_CalcLocalization      (SSpSourceReference     inSourceReference,
                                 SSpListenerReference   inListenerReference,
                                 SSpLocalizationData *  out3DInfo);

EXTERN_API_C( OSStatus )
SSpSource_SetTransform          (SSpSourceReference     inSourceReference,
                                 const TQ3Matrix4x4 *   inTransform);

EXTERN_API_C( OSStatus )
SSpSource_GetTransform          (SSpSourceReference     inSourceReference,
                                 TQ3Matrix4x4 *         outTransform);

EXTERN_API_C( OSStatus )
SSpSource_SetPosition           (SSpSourceReference     inSourceReference,
                                 const TQ3Point3D *     inPosition);

EXTERN_API_C( OSStatus )
SSpSource_GetPosition           (SSpSourceReference     inSourceReference,
                                 TQ3Point3D *           outPosition);

EXTERN_API_C( OSStatus )
SSpSource_SetOrientation        (SSpSourceReference     inSourceReference,
                                 const TQ3Vector3D *    inOrientation);

EXTERN_API_C( OSStatus )
SSpSource_GetOrientation        (SSpSourceReference     inSourceReference,
                                 TQ3Vector3D *          outOrientation);

EXTERN_API_C( OSStatus )
SSpSource_SetUpVector           (SSpSourceReference     inSourceReference,
                                 const TQ3Vector3D *    inUpVector);

EXTERN_API_C( OSStatus )
SSpSource_GetUpVector           (SSpSourceReference     inSourceReference,
                                 TQ3Vector3D *          outUpVector);

EXTERN_API_C( OSStatus )
SSpSource_SetCameraPlacement    (SSpSourceReference     inSourceReference,
                                 const TQ3CameraPlacement * inCameraPlacement);

EXTERN_API_C( OSStatus )
SSpSource_GetCameraPlacement    (SSpSourceReference     inSourceReference,
                                 TQ3CameraPlacement *   outCameraPlacement);

EXTERN_API_C( OSStatus )
SSpSource_SetVelocity           (SSpSourceReference     inSourceReference,
                                 const TQ3Vector3D *    inVelocity);

EXTERN_API_C( OSStatus )
SSpSource_GetVelocity           (SSpSourceReference     inSourceReference,
                                 TQ3Vector3D *          outVelocity);

EXTERN_API_C( OSStatus )
SSpSource_GetActualVelocity     (SSpSourceReference     inSourceReference,
                                 TQ3Vector3D *          outVelocity);

EXTERN_API_C( OSStatus )
SSpSource_SetCPULoad            (SSpSourceReference     inSourceReference,
                                 UInt32                 inCPULoad);

EXTERN_API_C( OSStatus )
SSpSource_GetCPULoad            (SSpSourceReference     inSourceReference,
                                 UInt32 *               outCPULoad);

EXTERN_API_C( OSStatus )
SSpSource_SetMode               (SSpSourceReference     inSourceReference,
                                 UInt32                 inMode);

EXTERN_API_C( OSStatus )
SSpSource_GetMode               (SSpSourceReference     inSourceReference,
                                 UInt32 *               outMode);

EXTERN_API_C( OSStatus )
SSpSource_SetReferenceDistance  (SSpSourceReference     inSourceReference,
                                 float                  inReferenceDistance);

EXTERN_API_C( OSStatus )
SSpSource_GetReferenceDistance  (SSpSourceReference     inSourceReference,
                                 float *                outReferenceDistance);

EXTERN_API_C( OSStatus )
SSpSource_SetSize               (SSpSourceReference     inSourceReference,
                                 float                  inLength,
                                 float                  inWidth,
                                 float                  inHeight);

EXTERN_API_C( OSStatus )
SSpSource_GetSize               (SSpSourceReference     inSourceReference,
                                 float *                outLength,
                                 float *                outWidth,
                                 float *                outHeight);

EXTERN_API_C( OSStatus )
SSpSource_SetAngularAttenuation (SSpSourceReference     inSourceReference,
                                 float                  inConeAngle,
                                 float                  inConeAttenuation);

EXTERN_API_C( OSStatus )
SSpSource_GetAngularAttenuation (SSpSourceReference     inSourceReference,
                                 float *                outConeAngle,
                                 float *                outConeAttenuation);

/*******************************************************************************
 *  OpenGL Calling Convention Call Variants
 ******************************************************************************/
EXTERN_API_C( OSStatus )
SSpListener_SetTransformfv      (SSpListenerReference   inListenerReference,
                                 const float *          inTransform);

EXTERN_API_C( OSStatus )
SSpListener_GetTransformfv      (SSpListenerReference   inListenerReference,
                                 float *                outTransform);

EXTERN_API_C( OSStatus )
SSpListener_SetPosition3f       (SSpListenerReference   inListenerReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpListener_SetPositionfv       (SSpListenerReference   inListenerReference,
                                 const float *          inPosition);

EXTERN_API_C( OSStatus )
SSpListener_GetPositionfv       (SSpListenerReference   inListenerReference,
                                 float *                outPosition);

EXTERN_API_C( OSStatus )
SSpListener_SetOrientation3f    (SSpListenerReference   inListenerReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpListener_SetOrientationfv    (SSpListenerReference   inListenerReference,
                                 const float *          inOrientation);

EXTERN_API_C( OSStatus )
SSpListener_GetOrientationfv    (SSpListenerReference   inListenerReference,
                                 float *                outOrientation);

EXTERN_API_C( OSStatus )
SSpListener_SetUpVector3f       (SSpListenerReference   inListenerReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpListener_SetUpVectorfv       (SSpListenerReference   inListenerReference,
                                 const float *          inUpVector);

EXTERN_API_C( OSStatus )
SSpListener_GetUpVectorfv       (SSpListenerReference   inListenerReference,
                                 float *                outUpVector);

EXTERN_API_C( OSStatus )
SSpListener_SetCameraPlacementfv (SSpListenerReference  inListenerReference,
                                 const float *          inCameraLocation,
                                 const float *          inPointOfInterest,
                                 const float *          inUpVector);

EXTERN_API_C( OSStatus )
SSpListener_GetCameraPlacementfv (SSpListenerReference  inListenerReference,
                                 float *                outCameraPlacement,
                                 float *                outPointOfInterest,
                                 float *                outUpVector);

EXTERN_API_C( OSStatus )
SSpListener_SetVelocity3f       (SSpListenerReference   inListenerReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpListener_SetVelocityfv       (SSpListenerReference   inListenerReference,
                                 const float *          inVelocity);

EXTERN_API_C( OSStatus )
SSpListener_GetVelocityfv       (SSpListenerReference   inListenerReference,
                                 float *                outVelocity);

EXTERN_API_C( OSStatus )
SSpListener_GetActualVelocityfv (SSpListenerReference   inListenerReference,
                                 float *                outVelocity);

EXTERN_API_C( OSStatus )
SSpSource_SetTransformfv        (SSpSourceReference     inSourceReference,
                                 const float *          inTransform);

EXTERN_API_C( OSStatus )
SSpSource_GetTransformfv        (SSpSourceReference     inSourceReference,
                                 float *                outTransform);

EXTERN_API_C( OSStatus )
SSpSource_SetPosition3f         (SSpSourceReference     inSourceReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpSource_SetPositionfv         (SSpSourceReference     inSourceReference,
                                 const float *          inPosition);

EXTERN_API_C( OSStatus )
SSpSource_GetPositionfv         (SSpSourceReference     inSourceReference,
                                 float *                outPosition);

EXTERN_API_C( OSStatus )
SSpSource_SetOrientation3f      (SSpSourceReference     inSourceReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpSource_SetOrientationfv      (SSpSourceReference     inSourceReference,
                                 const float *          inOrientation);

EXTERN_API_C( OSStatus )
SSpSource_GetOrientationfv      (SSpSourceReference     inSourceReference,
                                 float *                outOrientation);

EXTERN_API_C( OSStatus )
SSpSource_SetUpVector3f         (SSpSourceReference     inSourceReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpSource_SetUpVectorfv         (SSpSourceReference     inSourceReference,
                                 const float *          inUpVector);

EXTERN_API_C( OSStatus )
SSpSource_GetUpVectorfv         (SSpSourceReference     inSourceReference,
                                 float *                outUpVector);

EXTERN_API_C( OSStatus )
SSpSource_SetCameraPlacementfv  (SSpSourceReference     inSourceReference,
                                 const float *          inCameraLocation,
                                 const float *          inPointOfInterest,
                                 const float *          inUpVector);

EXTERN_API_C( OSStatus )
SSpSource_GetCameraPlacementfv  (SSpSourceReference     inSourceReference,
                                 float *                outCameraPlacement,
                                 float *                outPointOfInterest,
                                 float *                outUpVector);

EXTERN_API_C( OSStatus )
SSpSource_SetVelocity3f         (SSpSourceReference     inSourceReference,
                                 float                  inX,
                                 float                  inY,
                                 float                  inZ);

EXTERN_API_C( OSStatus )
SSpSource_SetVelocityfv         (SSpSourceReference     inSourceReference,
                                 const float *          inVelocity);

EXTERN_API_C( OSStatus )
SSpSource_GetVelocityfv         (SSpSourceReference     inSourceReference,
                                 float *                outVelocity);

EXTERN_API_C( OSStatus )
SSpSource_GetActualVelocityfv   (SSpSourceReference     inSourceReference,
                                 float *                outVelocity);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_CPU_PPC */


/*******************************************************************************
 *    LATE-BREAKING NEWS
 *
 *    After the documentation was completed, it was decided that the SSpSetup
 *  were not specific enough.  We renamed them to SSpSpeakerSetup.  These
 *    #defines allow code to be written per the documentation.  But please use
 * the new, longer names, as the #defines will be removed in a later release.
 ******************************************************************************/
#define SSpConfigureSetup       SSpConfigureSpeakerSetup

#define siSSpSetup                siSSpSpeakerSetup
#define SSpSetupData          SSpSpeakerSetupData


/*******************************************************************************
 *  MORE LATE-BREAKING NEWS
 *
 *  The SndGetInfo selector siSSpFilterVersion and datatype SSpFilterVersionData
 *  have been removed in favor of an alternate way of accessing filter version
 *  information.  The following function may be used for this purpose.
 *******************************************************************************
// **************************** GetSSpFilterVersion ****************************
// Finds the manufacturer and version number of the SoundSprocket filter that
// may be installed.  inManufacturer should be the manufacturer code specified
// at the installation time, which may be zero to allow any manufacturer.
// If no error is encountered, outManufacturer is set to the actual manufacturer
// code and outMajorVersion and outMinorVersion are set to the component
// specification level and manufacturer's implementation revision, respectively.
OSStatus GetSSpFilterVersion(
    OSType                  inManufacturer,
    OSType*                 outManufacturer,
    UInt32*                 outMajorVersion,
    UInt32*                 outMinorVersion)
{
    OSStatus                err;
    ComponentDescription    description;
    Component               componentRef;
    UInt32                  vers;
    
    // Set up the component description
    description.componentType           = kSoundEffectsType;
    description.componentSubType        = kSSpLocalizationSubType;
    description.componentManufacturer   = inManufacturer;
    description.componentFlags          = 0;        
    description.componentFlagsMask      = 0;    
    
    // Find a component matching the description
    componentRef = FindNextComponent(nil, &description);
    if (componentRef == nil)  return couldntGetRequiredComponent;
    
    // Get the component description (for the manufacturer code)
    err = GetComponentInfo(componentRef, &description, nil, nil, nil);
    if (err != noErr)  return err;
    
    // Get the version composite
    vers = (UInt32) GetComponentVersion((ComponentInstance) componentRef);
    
    // Return the results
    *outManufacturer = description.componentManufacturer;
    *outMajorVersion = HiWord(vers);
    *outMinorVersion = LoWord(vers);
    
    return noErr;
}
*******************************************************************************/



#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SOUNDSPROCKET__ */

