package com.sample.textureview;

import static android.hardware.camera2.CameraMetadata.LENS_FACING_BACK;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.graphics.Matrix;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.util.Size;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.widget.FrameLayout;

import java.io.File;

public class ViewActivity
    extends Activity implements TextureView.SurfaceTextureListener,
                                ActivityCompat.OnRequestPermissionsResultCallback {

    long ndkCamera_;
    private TextureView textureView_;

    Surface surface_ = null;
    SurfaceTexture surfaceTexture_ = null;
    boolean m_isStarted = false;
    boolean m_debug_run = false;

    private Size cameraPreviewSize_;

    @Override public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        onWindowFocusChanged(true);
        setContentView(R.layout.activity_main);
        if (isCamera2Device()) {
            RequestCamera();
        } else {
            Log.e("CameraPreview",
                "Found legacy camera device, this app only works with camera2 device");
        }
    }

    @Override public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
        }
    }

    private boolean isCamera2Device()
    {
        CameraManager camMgr = (CameraManager)getSystemService(Context.CAMERA_SERVICE);
        boolean camera2Dev = true;
        try {
            String[] cameraIds = camMgr.getCameraIdList();
            if (cameraIds.length != 0) {
                for (String id : cameraIds) {
                    CameraCharacteristics characteristics = camMgr.getCameraCharacteristics(id);
                    int deviceLevel
                        = characteristics.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL);
                    int facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                    if (deviceLevel == CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY
                        && facing == LENS_FACING_BACK) {
                        camera2Dev = false;
                    }
                }
            }
        } catch (CameraAccessException e) {
            e.printStackTrace();
            camera2Dev = false;
        }
        return camera2Dev;
    }

    private void createTextureView()
    {
        textureView_ = (TextureView)findViewById(R.id.texturePreview);
        textureView_.setSurfaceTextureListener(this);
        if (textureView_.isAvailable()) {
            onSurfaceTextureAvailable(textureView_.getSurfaceTexture(), textureView_.getWidth(),
                textureView_.getHeight());
        }

        textureView_.setOnLongClickListener(v -> {
            Log.e("CameraPreview", "onLongClick");
            m_debug_run = !m_debug_run;
            debugRun(ndkCamera_, m_debug_run);
            return true;
        });

        textureView_.setOnClickListener(v -> {
            Log.e("CameraPreview", "onClick");
            m_isStarted = !m_isStarted;
            startPreview(ndkCamera_, m_isStarted);
        });

    }

    public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int width, int height)
    {
        createNativeCamera();

        resizeTextureView(width);
        surfaceTexture_ = surfaceTexture;
        surfaceTexture_.setDefaultBufferSize(
            cameraPreviewSize_.getWidth(), cameraPreviewSize_.getHeight());

        File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS);
        surface_ = new Surface(surfaceTexture);
        onPreviewSurfaceCreated(ndkCamera_, surface_, path.getPath());
        m_isStarted = true;
    }

    private void resizeTextureView(int textureWidth)
    {
        int rotation = getWindowManager().getDefaultDisplay().getRotation();
        int newWidth = textureWidth;
        int newHeight
            = textureWidth * cameraPreviewSize_.getWidth() / cameraPreviewSize_.getHeight();

        if (Surface.ROTATION_90 == rotation || Surface.ROTATION_270 == rotation) {
            newHeight
                = (textureWidth * cameraPreviewSize_.getHeight()) / cameraPreviewSize_.getWidth();
        }
        textureView_.setLayoutParams(
            new FrameLayout.LayoutParams(newWidth, newHeight, Gravity.CENTER));
        configureTransform(newWidth, newHeight);
    }

    void configureTransform(int width, int height)
    {
        int mDisplayOrientation = getWindowManager().getDefaultDisplay().getRotation() * 90;
        Matrix matrix = new Matrix();
        if (mDisplayOrientation % 180 == 90) {
            // Rotate the camera preview when the screen is landscape.
            matrix.setPolyToPoly(
                new float[] {
                    0.f, 0.f, // top left
                    width, 0.f, // top right
                    0.f, height, // bottom left
                    width, height, // bottom right
                },
                0,
                mDisplayOrientation == 90 ?
                                          // Clockwise
                    new float[] {
                        0.f, height, // top left
                        0.f, 0.f, // top right
                        width, height, // bottom left
                        width, 0.f, // bottom right
                    }
                                          : // mDisplayOrientation == 270
                    // Counter-clockwise
                    new float[] {
                        width, 0.f, // top left
                        width, height, // top right
                        0.f, 0.f, // bottom left
                        0.f, height, // bottom right
                    },
                0, 4);
        } else if (mDisplayOrientation == 180) {
            matrix.postRotate(180, width / 2, height / 2);
        }
        textureView_.setTransform(matrix);
    }

    private void createNativeCamera()
    {
        Display display = getWindowManager().getDefaultDisplay();
        int height = display.getMode().getPhysicalHeight();
        int width = display.getMode().getPhysicalWidth();

        ndkCamera_ = createCamera(width, height);
        cameraPreviewSize_ = getMinimumCompatiblePreviewSize(ndkCamera_);
    }

    private static final int PERMISSION_REQUEST_CODE_CAMERA = 1;
    public void RequestCamera()
    {
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
            != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                this, new String[] { Manifest.permission.CAMERA }, PERMISSION_REQUEST_CODE_CAMERA);
            return;
        }
        createTextureView();
    }

    @Override public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {
            Log.i("Set finder - java", "save image request");
            saveImage(ndkCamera_);
        }
        return true;
    }

    /*
        SurfaceTextureListener overrides
    */
    @Override public boolean onSurfaceTextureDestroyed(SurfaceTexture surface)
    {
        onPreviewSurfaceDestroyed(ndkCamera_, surface_);
        deleteCamera(ndkCamera_);
        ndkCamera_ = 0;
        surface_ = null;
        return true;
    }

    @Override
    public void onRequestPermissionsResult(
        int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults)
    {
        if (PERMISSION_REQUEST_CODE_CAMERA != requestCode) {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
            return;
        }

        if (grantResults.length == 1 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            Thread initCamera = new Thread(() -> runOnUiThread(this::createTextureView));
            initCamera.start();
        }
    }

    @Override public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height)
    {
    }

    @Override public void onSurfaceTextureUpdated(SurfaceTexture surface) { }

    /*
        NATIVE METHODS
     */
    private native long createCamera(int width, int height);

    private native Size getMinimumCompatiblePreviewSize(long ndkCamera);

    private native void onPreviewSurfaceCreated(long ndkCamera, Surface surface, String save_path);

    private native void onPreviewSurfaceDestroyed(long ndkCamera, Surface surface);

    private native void deleteCamera(long ndkCamera);

    private native void startPreview(long ndkCamera, boolean start);

    private native void saveImage(long ndkCamera);

    private native void debugRun(long ndkCamera, boolean debug_run);

    static { System.loadLibrary("camera_textureview"); }
}
