
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "VideoCaptureViewController.h"
#import "CameraCalibration.h"

#define ARDOOR_MODE_NONE 0
#define ARDOOR_MODE_EDGES 1
#define ARDOOR_MODE_CALIBRATION 2
#define ARDOOR_EDGE_MODE_NONE 0
#define ARDOOR_EDGE_MODE_CANNY 1
#define ARDOOR_EDGE_MODE_SOBEL 2

@interface VideoCaptureViewController : UIViewController <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    AVCaptureSession *_captureSession;
    AVCaptureDevice *_captureDevice;
    AVCaptureVideoDataOutput *_videoOutput;
    AVCaptureVideoPreviewLayer *_videoPreviewLayer;
    
    int _camera;
    NSString *_qualityPreset;
    BOOL _captureGrayscale;
    unsigned char _mode;
    unsigned char _edgeMode;
    
    // Fps calculation
    CMTimeValue _lastFrameTimestamp;
    float *_frameTimes;
    int _frameTimesIndex;
    int _framesToAverage;
    
    float _captureQueueFps;
    float _fps;
    
    // Debug UI
    UILabel *_fpsLabel;
    
    UIImage *_edgeImage;
    CALayer *_featureLayer;
    
    ARDoor::CameraCalibration *_calibrator;
}

// Current frames per second
@property (nonatomic, readonly) float fps;
@property (nonatomic, assign) BOOL showDebugInfo;
@property (nonatomic, assign) BOOL torchOn;

// AVFoundation components
@property (nonatomic, readonly) AVCaptureSession *captureSession;
@property (nonatomic, readonly) AVCaptureDevice *captureDevice;
@property (nonatomic, readonly) AVCaptureVideoDataOutput *videoOutput;
@property (nonatomic, readonly) AVCaptureVideoPreviewLayer *videoPreviewLayer;

// -1: default, 0: back camera, 1: front camera
@property (nonatomic, assign) int camera;

@property (nonatomic, assign) NSString * const qualityPreset;
@property (nonatomic, assign) BOOL captureGrayscale;

- (IBAction)toggleFps:(id)sender;
- (IBAction)toggleTorch:(id)sender;
- (IBAction)toggleCamera:(id)sender;
- (IBAction)toggleEdges:(id)sender;
- (IBAction)toggleCalibration:(id)sender;

- (CGAffineTransform)affineTransformForVideoFrame:(CGRect)videoFrame orientation:(AVCaptureVideoOrientation)videoOrientation;

@end
