import keyboard
from collections import deque
import cv2
import numpy as np
import os

video = cv2.VideoCapture("uda.mp4")

prev_p = 0
pause = 1
ok = 1

# Lists that will contain the lane points
poly_x = []
poly_y = []

# Variables that will hold the sum of the polynomial over the last n frames
s_rad1 = 0
s_rad2 = 0

# Create deques that will help averaging the polynomial's coefficients over
# a number of frames.
poly_avg_a1 = deque()
poly_avg_b1 = deque()
poly_avg_c1 = deque()

poly_avg_a2 = deque()
poly_avg_b2 = deque()
poly_avg_c2 = deque()

# Create two deques that will help in averaging the radius of the lanes over the
# last "n" frames (NOTE: The number of frames, "n", will be declared later in the code.)
rad1 = deque()
rad2 = deque()


# Initial image dimensions
height, width = (720, 1280)

region_of_interest_vertices = [
    (237, 688),
    ### 543, 443
    (570, 425),
    ### 443 in loc de 400
    ### 727, 443
    (700, 425),
    (1169, 688),
]

# Create the array of points that will help warp the image.
# The first array refers to the initial image and the second one
# specifies each pixel's location in the resulting image
pts1 = np.float32([[573, 443], [690, 443],[1117, 688], [282, 688]])
pts2 = np.float32([[573, 443], [690, 443],[690, 688], [573, 688]])

# Testing purposes only... . When the method is called, the viedo will be paused,
# in order to make it easier to observe the current image in the video.
def changePause():
    global pause
    if pause == 1:
        pause = 0
    else:
        pause = 1


def append_poly_point(image, a_h, a_w, win_h, win_w):
    # poly_x and poly_y are two lists that contain the coordinates of the white
    # points in a lane; this lists will be used to obtain a function that fits
    # the lane.
    global poly_x, poly_y

    # We will use the sliding window technique - a rectangle with a predefined
    # width and height will slide from the bottom of the image, up to the top,
    # searching for white pixels. All the white pixels are then appended to the
    # two arrays, "poly_x" and "poly_y"
    win_h = 16
    win_w = 50

    # Find all the true (white) values that fall under the area of the sliding window.
    # np.arghwere returns a list of coordinates corresponding to each white point
    coords = np.argwhere(image[a_h - win_h : a_h, a_w - win_w//2 : a_w + win_w//2])

    # Sum up the x values and the y values of all the white points
    h_sum = np.sum(coords[:,0], axis = 0)
    w_sum = np.sum(coords[:,1], axis = 0)

    if not (coords.size == 0):

        # Get the centre point coordinates in order to know (approximatively)
        # where the next sliding window should be centered
        centre_h = h_sum//(coords.size/2)
        centre_w = w_sum//(coords.size/2)

        # Transform the coordinates so that the reference system is the whole image,
        # not only the window.
        nch = int(a_h - centre_h)
        ncw = int(a_w - win_w//2 + centre_w)

        # Get the coordinates of all the white points
        x = [a_h - i for i in coords[:,0]]
        y = [a_w - win_w//2 + i for i in coords[:,1]]

        # Append the coordinates of all the points to the arrays that will be
        # then passed to the least squares function
        if not(nch == 0 or ncw == 0):
            poly_y = poly_y + y
            poly_x = poly_x + x

        # print("OK*")

        # Return the x coordinate of the "centre of mass"
        return ncw
    else:
        # print("NT*")
        return a_w

def region_of_interest(img, vertices, ch_count):
    # Define a blank matrix that matches the image height/width.
    mask = np.zeros_like(img)

    if (ch_count == None):
        # Retrieve the number of color channels of the image.
        channel_count = img.shape[2]
    else:
        channel_count = ch_count
    # Create a match color with the same color channel counts.
    match_mask_color = (255,) * channel_count

    # Fill inside the polygon
    cv2.fillPoly(mask, vertices, match_mask_color)

    # Returning the image only where mask pixels match
    masked_image = cv2.bitwise_and(img, mask)
    return masked_image

def pre_processing(image):

    image = cv2.resize(image, (1280, 720))

    # Apply bilateral blur on the image, in order to reduce noise.
    # image = cv2.bilateralFilter(image,3,75,75)

    image = image.copy()

    # Obtain only the area where lanes could appear, black out the rest of
    # the image.
    image = region_of_interest(
        image,
        np.array([region_of_interest_vertices], np.int32),
        None
    )

    # Convert the image into HSL
    hls = cv2.cvtColor(image, cv2.COLOR_BGR2HLS)

    # Set the thresholds for the white lane color
    # The colorspace is HUE, LUM, SAT
    lower_white = np.array([0  ,200  ,0 ])
    upper_white = np.array([179,255,255])

    # Set the thresholds for the yellow lane color
    lower_yellow = np.array([15 ,100 ,100])
    upper_yellow = np.array([35,255,255])

    # Obtain two masks of the image, one that contains only the white lane and
    # one that contains only the yellow image; combine the two masks.
    # The mask will be a matrix filled with zeroes
    mask1 = cv2.inRange(hls, lower_yellow, upper_yellow)
    mask2 = cv2.inRange(hls, lower_white, upper_white)
    mask = cv2.bitwise_or(mask1, mask2, mask = None)

    # Apply the combined mask over the image, in order to obtain a color image
    # where only the lanes are visible.

    # The res image will not be returned or used later.
    res = cv2.bitwise_and(image, image, mask = mask)

    # Aflu ca res si mask sunt de tipul numpy.ndarray
    # print(res.type())
    # print(mask.type())

    return mask

def warping(lane_mask):
    IMH = 688

    rows,cols = mask.shape

    # Get the transformation matrix and the inverse transformation matrix
    # (the first one is used to warp the image, the second one to unwarp it)
    # The transformatioin matrix will be used in this function while the inverse
    # matrix will be returned to the main program in order to be used later.
    M = cv2.getPerspectiveTransform(pts1,pts2)
    Minv = cv2.getPerspectiveTransform(pts2, pts1)

    IMH, IMW =  mask.shape
    # Warp the image and then unwarp it
    dst = cv2.warpPerspective(mask,M,(IMW, IMH))

    unwarped_mask_full = cv2.warpPerspective(dst,Minv,(IMW, IMH))

    warped_mask_full = dst.copy()
    # Crop the image only the bottom part of it remains
    dst = dst[443:688,  237:1169]

    return dst, unwarped_mask_full, warped_mask_full, Minv

# Fits a curve on the right and left lanes of the road.
def fit_curve(image):
    # "poly_x" and "poly_y" contain the white points' coordinates
    # These two arrays will be passed to the polyfit function in order to
    # obtain the coefficients of the polynomial function that best matches the lane
    global poly_x, poly_y
    global sum_a, sum_b, sum_c

    # Get the image height and width
    image_h, image_w = image.shape

    # Make a histogram of the white pixel values
    histogram = np.sum(image[(image.shape[0]//10):,:], axis=0)

    # Reset the two point lists
    del poly_x[0:len(poly_x) - 1]
    del poly_y[0:len(poly_y) - 1]

    anchor_y = image_h
    anchor_x = np.argmax(histogram[:400])

    # Obtain the left lane border points
    while (anchor_y - 16 > 0):
        # Change the anchor point's coordinates and call the function that
        # populates the lists of coordinates
        anchor_x = append_poly_point(image, anchor_y, anchor_x, 30, 30)
        anchor_y -= 15

    # Get the coefficients of the polynomial function that best fits the lane
    a1,b1,c1 = np.polyfit(poly_x, poly_y, 2)

    # Reset the two point lists
    del poly_x[0:len(poly_x) - 1]
    del poly_y[0:len(poly_y) - 1]

    # Reset the anchor point coordinates
    anchor_y = 245
    anchor_x = np.argmax(histogram[400:]) + 400

    # Search for pixels in the right lane border
    while (anchor_y - 16 > 0):
        # Change the anchor point's coordinates and call the function that
        # populates the lists of coordinates
        anchor_x = append_poly_point(image, anchor_y, anchor_x, 30, 30)
        anchor_y -= 15

    # Get the coefficients of the polynomial function that best fits the lane
    a2,b2,c2 = np.polyfit(poly_x, poly_y, 2)


    return a1, b1, c1, a2, b2, c2

# Generates an array of points that defines the polygon between the two lanes.
def generate_polygon_vertices(a1, b1, c1, a2, b2, c2):
    # Create the array of points that defines the left lane.
    left_side = [[int(237 + a1*(x**2) + b1*x + c1),443 +  x] for x in range(245, -1, -1)]

    # Create the array of points that defines the left lane.
    right_side = [[int(237 + a2*(x**2) + b2*x + c2),443 + x] for x in range(0, 245)]

    # Create the polygon that defines the current lane
    # Convert the polygon to a numpy array
    polygon_vertices = left_side + right_side
    polygon_vertices = np.array(polygon_vertices)

    return polygon_vertices

# "error_region_cut" defines a trapeze that will trim the polygon that fills
# the space in between the two lanes
error_region_cut = [
    (237, 688),
    # (502, 476),
    # (783, 476),
    (502, 400),
    (783, 400),
    (1169, 688),
]

# Fills the region between lanes with a color
# Receives an array of points, that represents the polygon to be filled, an
def fill_lane_and_unwarp(polygon_vertices, warped_mask_full):

    # Fill the lane area in the mask with white
    cv2.fillPoly(warped_mask_full, np.int32([polygon_vertices]), 255)

    # Unwarp the mask and save it into a new matrix
    IMH, IMW =  mask.shape

    unwarped_mask_full = cv2.warpPerspective(warped_mask_full,Minv,(IMW, IMH))

    #cv2.imshow('warped', unwarped_mask_full)

    unwarped_mask_full = region_of_interest(unwarped_mask_full, np.array([error_region_cut], np.int32), 1)

    # Apply a bitwise_and operation on the color image, based on the
    # pixel mask. It should fill the area where there are white pixels with
    # a shade of green.

    # Note: the color image is the cropped one (the one that has the area where
    # lanes are not expected to appear blacked out).
    lane_marked = cv2.bitwise_and(image, (255,255,0), mask = unwarped_mask_full)

    # I don't know for sure what bitwise_or makes, but I obtain the desired result
    lane_marked = cv2.bitwise_xor(lane_marked, image, mask = None)

    return lane_marked

sum_a1 = 0
sum_b1 = 0
sum_c1 = 0

sum_a2 = 0
sum_b2 = 0
sum_c2 = 0

# Averages the 6 coefficents of the polynomials that define the
def average_coeffs(a1, b1, c1, a2, b2, c2):
    global sum_a1, sum_b1, sum_c1
    global sum_a2, sum_b2, sum_c2

    avg_n = 10

    if (len(poly_avg_a1) > avg_n):
        sum_a1 -= poly_avg_a1.popleft()
        sum_b1 -= poly_avg_b1.popleft()
        sum_c1 -= poly_avg_c1.popleft()

        sum_a2 -= poly_avg_a2.popleft()
        sum_b2 -= poly_avg_b2.popleft()
        sum_c2 -= poly_avg_c2.popleft()

    poly_avg_a1.append(a1)
    poly_avg_b1.append(b1)
    poly_avg_c1.append(c1)

    poly_avg_a2.append(a2)
    poly_avg_b2.append(b2)
    poly_avg_c2.append(c2)

    sum_a1 += a1
    sum_b1 += b1
    sum_c1 += c1

    sum_a2 += a2
    sum_b2 += b2
    sum_c2 += c2

    a1 = sum_a1/len(poly_avg_a1)
    b1 = sum_b1/len(poly_avg_b1)
    c1 = sum_c1/len(poly_avg_c1)

    a2 = sum_a2/len(poly_avg_a2)
    b2 = sum_b2/len(poly_avg_b2)
    c2 = sum_c2/len(poly_avg_c2)

    return a1, b1, c1, a2, b2, c2

def calculate_radius(a1, b1, c1, a2, b2, c2, x):
    # Calculate the radius of each of the two sides of the lane
    radius1 = ((1 + (2*a1*x + b1)**2)**(3/2))/abs(2*a1)
    radius2 = ((1 + (2*a2*x + b2)**2)**(3/2))/abs(2*a2)

    return radius1, radius2

### MAIN #######################################################################

count = 0
labcount = 0
while(video.isOpened()):
    if pause:
        # Take the images from the video, one by one
        ret, image = video.read()

        raw_image = cv2.cvtColor(image, cv2.COLOR_BGR2HLS)

        # Pass the image to the pre-processing function that filters everything,
        # except the image zones that are white. The lanes should still appear
        # in the images, since they are white.
        mask = pre_processing(image)

        ### CODE ONLY FOR LANE COLOR CHECKING
        # IMH, IMW =  mask.shape
        #
        # M = cv2.getPerspectiveTransform(pts1,pts2)
        # # Warp the image and then unwarp it
        # warped_image_full =  cv2.warpPerspective(image,M,(IMW, IMH))
        ### END - CODE ONLY FOR LANE COLOR CHECKING
        cropped_warped_mask, unwarped_mask_full, warped_mask_full, Minv = warping(mask)

        # cv2.imshow('dst', cv2.resize(dst,(720,420)))
        # cv2.imshow('unw', cv2.resize(unwarped_mask_full,(720,420)))
        # cv2.imshow('warp', cv2.resize(warped_mask_full,(720,420)))
        # cv2.imshow('wapred_image', cv2.resize(warped_image_full,(720,420)))
        # cv2.imshow('raw_image', cv2.resize(raw_image,(720,420)))

        # labcount += 1
        # cv2.imwrite('labimg{}.tiff'.format(labcount), raw_image)

        # The problem here is that the coefficients AND the polygon_vertices array
        # are both computed in the fit_curve function, which must be either divided
        # into smaller parts either the averaging algorithm must be placed outside
        # the function.
        a1, b1, c1, a2, b2, c2 = fit_curve(cropped_warped_mask)

        # Nu stiu cum
        a1, b1, c1, a2, b2, c2 = average_coeffs(a1, b1, c1, a2, b2, c2)

        polygon_vertices = generate_polygon_vertices(a1, b1, c1, a2, b2, c2)

        # The polygon filling must be separated from the image unwarping
        lane_marked = fill_lane_and_unwarp(polygon_vertices, warped_mask_full)

        # unwarp(warped_mask_full)

        values = [240, 200, 150, 100, 50]

        x = 240

        avg_rad = 20

        radius1, radius2 = calculate_radius(a1, b1, c1, a2, b2, c2, x)

        # Add the radius to the sum that will be averaged
        if (len(rad1) > avg_rad):
            s_rad1 -= rad1.popleft()
            s_rad2 -= rad2.popleft()

        rad1.append(radius1)
        rad2.append(radius2)

        s_rad1 += radius1
        s_rad2 += radius2

        radius1 = s_rad1/len(rad1)
        radius2 = s_rad2/len(rad2)

        text = "Road radius: {}".format(radius2 - radius1)

        image = cv2.putText(lane_marked, text, (0,x), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 255, 255), lineType=cv2.LINE_AA)
        # for x in values:
        #
        #     radius1 = ((1 + (2*a1*x + b1)**2)**(3/2))/abs(2*a1)
        #     radius2 = ((1 + (2*a2*x + b2)**2)**(3/2))/abs(2*a2)
        #     # text += " " + str((radius1 + radius2)/2)
        #     text = " " + str(radius1)
        #     text += "           " + str(radius2)
        #
        #     image = cv2.putText(lane_marked, text, (0,x), cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 255, 255), lineType=cv2.LINE_AA)

        cv2.imshow('image', cv2.resize(lane_marked,(720,420)))

    else:
        cv2.imshow('unwarped_mask_full',  cv2.resize(unwarped_mask_full,(720,420)))



    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break
    elif k == ord('p'):
        if prev_p == 0:
            prev_p = 1
            # os.chdir(r"C:\Users\Beniamin\Desktop\Bosch\debugging_images\image")
            # cv2.imwrite('{}image.png'.format(count), raw_image)
            # os.chdir(r"C:\Users\Beniamin\Desktop\Bosch\debugging_images\dst")
            # cv2.imwrite('{}dst.png'.format(count), dst)
            # os.chdir(r"C:\Users\Beniamin\Desktop\Bosch\debugging_images\lane")
            # cv2.imwrite('{}lane.png'.format(count), image)
            # count += 1
            changePause()
    elif not (k == ord('p')):
        if prev_p == 1:
            prev_p = 0


video.release()
cv2.destroyAllWindows()
