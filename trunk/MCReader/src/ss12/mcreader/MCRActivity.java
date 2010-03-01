/*
 * MCRActivity.java
 * Mobile Currency Reader
 * SS12 February 2010
 * 
 * Based on example camera code at http://marakana.com/forums/android/android_examples/39.html
 * 
 */

package ss12.mcreader;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketException;
import java.net.URL;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.hardware.Camera.Size;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.FrameLayout;

public class MCRActivity extends Activity {
	private static final String TAG = "MCRActivity";
	
	Preview preview; // preview holds the camera

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		// this R.id stuff is in the main.xml file
		// create the surface that the camera preview is on
		if (preview == null) {
			preview = new Preview(this);
			((FrameLayout) findViewById(R.id.preview)).addView(preview);
			preview.setOnClickListener( new OnClickListener() {
				// takes picture when the screen is touched/clicked
				public void onClick(View v) {
					// set photo resolution as preparation for image processing
					Camera.Parameters parameters = preview.camera.getParameters();
					for (Size s : parameters.getSupportedPictureSizes()) {
						Log.d(TAG, "available size: " + String.valueOf(s.height) +","+ String.valueOf(s.width));
					}
					parameters.setPictureSize(Constants.PICTURE_WIDTH, Constants.PICTURE_HEIGHT);
					preview.camera.setParameters(parameters);
					preview.camera.takePicture(shutterCallback, rawCallback, jpegCallback);
				}
			});
		}
	}
	
	@Override
	public void onPause() {
		super.onPause();
		this.finish();
	}
	
	ShutterCallback shutterCallback = new ShutterCallback() {
		public void onShutter() {
			/* make shutter sound to signal that picture was taken */
			playAudio(Constants.SHUTTER_SOUND);
		}
	};

	/** Handles data for raw picture */
	PictureCallback rawCallback = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera camera) {
			Log.d(TAG, "onPictureTaken - raw");
		}
	};

	/** Handles data for jpeg picture */
	PictureCallback jpegCallback = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera camera) {
			/* sending image */
			try {
				Log.d(TAG, "START SENDING!!");
				onSendImageFile(data);
			} 
			catch (InterruptedException e) {
				e.printStackTrace();
			}
			catch (FileNotFoundException e) {
				Log.e("onCreate","file not found");
			}
			Log.d(TAG, "onPictureTaken - jpeg");
			
			preview.refresh();
		}
	};
	
	public void onSendImageFile(byte [] data) throws FileNotFoundException, InterruptedException {
    	Log.d(TAG,"onSendImageFile!");
    	String fileName = "/data/data/ss12.mcreader/files/test.jpeg";
    	//FileInputStream fileInputStream = new FileInputStream(new File(fileName));
    	HttpURLConnection conn = null;
        DataOutputStream dos = null;
        DataInputStream inStream = null;

        String lineEnd = "\r\n";
        String twoHyphens = "--";
        String boundary = "*****";

        String responseFromServer = "";

        String urlString = "http://acm.cs.ucla.edu/MCR/uploadMCR.php";

        Log.d(TAG,"starting file read");
        // loop waiting sound while waiting for response from server
        MediaPlayer mp = MediaPlayer.create(getBaseContext(), R.raw.loading);
        mp.setLooping(true);
		mp.start();
        try
        {
        //------------------ CLIENT REQUEST
        // open a URL connection to the Servlet
        URL url = new URL(urlString);

        // Open a HTTP connection to the URL
        conn = (HttpURLConnection) url.openConnection();
        Log.d("fileupload","Got connection");

        // Allow Inputs
        conn.setDoInput(true);

        // Allow Outputs
        conn.setDoOutput(true);

        // Don't use a cached copy.
        conn.setUseCaches(false);

        // Use a post method.
        conn.setRequestMethod("POST");
        conn.setRequestProperty("Connection", "Keep-Alive");
        conn.setRequestProperty("Content-Type", "multipart/form-data;boundary="+boundary);
        
        dos = new DataOutputStream( conn.getOutputStream() );
        dos.writeBytes(twoHyphens + boundary + lineEnd);
        dos.writeBytes("Content-Disposition: form-data; name=\"uploadedfile\";filename=\"" + fileName +"\"" + lineEnd);
        dos.writeBytes(lineEnd);
        Log.d("MediaPlayer","Headers are written");
        
        dos.write(data);

        // send multipart form data necesssary after file data...
        dos.writeBytes(lineEnd);
        dos.writeBytes(twoHyphens + boundary + twoHyphens + lineEnd);

        // close streams
        Log.d(TAG,"File is written");
        //fileInputStream.close();
        dos.flush();
        dos.close();

        }
        catch (MalformedURLException ex)
        {
        	Log.e(TAG, "error: " + ex.getMessage(), ex);
        }

        catch (IOException ioe)
        {
        	Log.e(TAG, "error: " + ioe.getMessage(), ioe);
        }


        //------------------ read the SERVER RESPONSE
        try {
            BufferedReader rd = new BufferedReader(new InputStreamReader(conn.getInputStream()));
            String line;
			// get response from server
        	line = rd.readLine();
            Log.d("Response", "Message: " + line);
            Integer returnedNum = Integer.valueOf(line);
            
			// play answer
            playAudio(returnedNum.intValue());
            rd.close();
        }
        catch (SocketException sockex) {
        	playAudio(Constants.CONNECTION_FAILED);
        	Log.e("MCR", "could not connect to network");
        }
        catch (IOException ioex){
        	Log.e("MCR", "error: " + ioex.getMessage(), ioex);
        }
        
        mp.setLooping(false);
		mp.release();
    }
	
	private void playAudio(int type) {
		try {
			MediaPlayer mp = null;
			
			switch (type) {
			
			// shutter sound plays after app takes a picture		
			case Constants.SHUTTER_SOUND:
				Log.i("Audio - grark", "playing camera shutter");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.shutter); 
		        mp.start();
				break;
				
			// error
			case -1:
				break;
			
			case Constants.ONE_DOLLAR_SOUND:
				Log.i("Audio - grark", "playing one dollar bill");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.one); 
		        mp.start();
				break;
			case Constants.FIVE_DOLLAR_SOUND:
				Log.i("Audio - grark", "playing five dollar bill");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.ten2); 
		        mp.start();
				break;
			case Constants.TEN_DOLLAR_SOUND:
				Log.i("Audio - grark", "playing ten dollar bill");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.ten); 
		        mp.start();
				break;
			case Constants.TWENTY_DOLLAR_SOUND:
				Log.i("Audio - grark", "playing twenty dollar bill");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.twenty); 
		        mp.start();
				break;
			case Constants.FIFTY_DOLLAR_SOUND:
				Log.i("Audio - grark", "playing fifty dollar bill");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.fifty); 
		        mp.start();
				break;
			case Constants.ONE_HUNDRED_DOLLAR_SOUND:
				Log.i("Audio - grark", "playing hundred dollar bill");
		        mp = MediaPlayer.create(getBaseContext(), R.raw.hundred); 
		        mp.start();
				break;
			case Constants.CONNECTION_FAILED:
				Log.i("Audio - matt", "cannot connect to network");
				mp = MediaPlayer.create(getBaseContext(), R.raw.errornetwork); 
		        mp.start();
		        break;
			default:
			}
		}
		catch (Exception ex) {
		}
	}
}