package vision

import (
	"image/color"
	"log"

	"gocv.io/x/gocv"
)

// VideoWindow manages a capture and display window.
type VideoWindow struct {
	Capture *gocv.VideoCapture // video source
	Window  *gocv.Window       // on-screen video display
	img     gocv.Mat           // current video frame
}

// NewVideoWindow creates a new VideoWindow for a given URL and title.
func NewVideoWindow(url string, title string) (*VideoWindow, error) {
	// create VideoCapture
	cap, err := gocv.VideoCaptureFile(url)
	if err != nil {
		return nil, err
	}

	// create GUI window
	win := gocv.NewWindow(title)

	return &VideoWindow{
		Capture: cap,
		Window:  win,
		img:     gocv.NewMat(),
	}, nil
}

// ReadFrame reads the next video frame.
func (vw *VideoWindow) ReadFrame() (gocv.Mat, bool) {
	// get next video frame
	if ok := vw.Capture.Read(&vw.img); !ok || vw.img.Empty() {
		return vw.img, false
	}

	return vw.img, true
}

// Display renders the video frame on the window and processes GUI events.
func (vw *VideoWindow) Display(img gocv.Mat) {
	vw.Window.IMShow(img)
	vw.Window.WaitKey(1)
}

// DetectGreen checks if green is present in a frame and logs pixel count.
func DetectGreen(img *gocv.Mat) bool {
	// convert BGR (format from Tello video frame) to HSV for better color detection
	hsv := gocv.NewMat()
	defer hsv.Close()
	gocv.CvtColor(*img, &hsv, gocv.ColorBGRToHSV)

	// HSV color ranges to detect green
	lower := gocv.NewScalar(30, 50, 50, 0)
	upper := gocv.NewScalar(90, 255, 255, 0)

	// create a binary mask such that pixels in range become white, with the rest becoming black
	mask := gocv.NewMat()
	defer mask.Close()
	gocv.InRangeWithScalar(hsv, lower, upper, &mask)

	// get pixel contours
	contours := gocv.FindContours(mask, gocv.RetrievalExternal, gocv.ChainApproxSimple)
	defer contours.Close()

	// track largest area
	maxArea := 0.0
	for i := 0; i < contours.Size(); i++ {
		area := gocv.ContourArea(contours.At(i))

		// update largest area
		if area > maxArea {
			maxArea = area
		}

		// draw rectangle on video frame
		if area > 100 {
			rect := gocv.BoundingRect(contours.At(i))
			gocv.Rectangle(img, rect, color.RGBA{0, 255, 0, 0}, 2)
		}
	}

	// log largest area
	if maxArea > 0.0 {
		log.Printf("[VISION] Largest Green Area: %.2f\n", maxArea)
	}

	return maxArea > 200
}

// Close releases all resources.
func (vw *VideoWindow) Close() {
	vw.Capture.Close()
	vw.Window.Close()
	vw.img.Close()
}
