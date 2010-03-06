package ss12.mcreader;

import android.app.ProgressDialog;
import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

class Preview extends SurfaceView implements SurfaceHolder.Callback {
	private static final String TAG = "Preview";
	public ProgressDialog dialog;
	
    SurfaceHolder mHolder;
    public Camera camera;
    
    Preview(Context context) {
        super(context);        
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        Log.e("PREVIEW", "Preview created");
        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }
    
    public void progressDialog () {
    	camera.stopPreview();
    	dialog = ProgressDialog.show(this.getContext(), "Processing", "Please wait for image to process...", true);
    }
    
    public void refresh() {
    	if (camera != null) {
    		Camera.Parameters parameters = camera.getParameters();
    		if (parameters != null && parameters.getSupportedPreviewSizes() != null) {
	            for (Size s : parameters.getSupportedPreviewSizes()) {
	            	Log.d("PREVIEW Available Sizes", s.width + "x" + s.height);
	            }
	            Camera.Size sz = parameters.getPreviewSize();
	            
	            parameters.setPreviewSize(Constants.PREVIEW_WIDTH, Constants.PREVIEW_HEIGHT);
	            //parameters.set("orientation", "portrait");
	            dialog.hide();
	            dialog = null;
	            //parameters.setPreviewSize(w, h);
	            camera.setParameters(parameters);
	            camera.startPreview();
    		}
    	}
    }

    public void surfaceCreated(SurfaceHolder holder) {
        // The Surface has been created, acquire the camera and tell it where
        // to draw.
    	Log.d("PREVIEW", "surface created");
    	if (camera == null)
    		camera = Camera.open();
        try {
			camera.setPreviewDisplay(holder);
		} catch (Exception e) {
			e.printStackTrace();
		}
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        // Surface will be destroyed when we return, so stop the preview.
        // Because the CameraDevice object is not a shared resource, it's very
        // important to release it when the activity is paused.
    	Log.d("PREVIEW", "Surface destroyed");
        camera.stopPreview();
        camera.release();
        camera = null;
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        // Now that the size is known, set up the camera parameters and begin
        // the preview.
    	Log.d("PREVIEW", "Surface changed");
    	Camera.Parameters parameters = camera.getParameters();
        //getSupportedPreviewSizes
    	int width = 0;
    	int height = 0;
    	for (Size s : parameters.getSupportedPreviewSizes()) {
			Log.d(TAG, "available size: " + String.valueOf(s.height) +","+ String.valueOf(s.width));
			width = s.width;
			height = s.height;
			if ( width > Constants.PICTURE_WIDTH_THRESHOLD || height > Constants.PICTURE_HEIGHT_THRESHOLD) {
				break;
			}
		}
        parameters.setPreviewSize(Constants.PREVIEW_WIDTH, Constants.PREVIEW_HEIGHT);
        //parameters.setPreviewSize(sz.width, sz.height);
        //parameters.set("orientation", "portrait");

        //parameters.setPreviewSize(w, h);
        camera.setParameters(parameters);
        camera.startPreview();
    }

    /*
    @Override
    public void draw(Canvas canvas) {
    		super.draw(canvas);
    		Paint p= new Paint(Color.RED);
    		Log.d(TAG,"draw");
    		canvas.drawText("PREVIEW", canvas.getWidth()/2, canvas.getHeight()/2, p );
    }
    */
}