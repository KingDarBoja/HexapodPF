# Hexapod Inverse Kinematics GUI
# Manuel Bojato, Universidad Del Norte, CO


# Import libraries and modules
from tkinter import *
from sympy import *
import numpy as np
# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D


def IK_Equations(geometry_values):
    # Angles and coordinates symbols used to represent hexapod kinematic.
    # - alpha, beta, gamma denotes the global body yaw, pitch, roll angles.
    # - a_k: Local yaw angle for body to paw rotation.
    # - r_t: Hexapod body radius.
    # - a_i: Radius of the new origin about the previous z.
    # - d_i: Offset or "depth" along the previous joint's z axis.
    # - theta_i: Rotation about the previous z.
    # - alpha_i: Rotation about the new x axis to align the old z to the new z.
    # - t_i: theta angles for each servo link (replaces theta_i for each paw).
    # - L_i: servo link lenghts (replaces a_i for each paw).
    # - x_b, y_b, z_b: Global to body center point coordinates translation.
    # - x_p, y_p, z_p: Origin coordinates of one paw.
    alpha, beta, gamma = symbols('alpha beta gamma')
    a_k, r_t, a_i, d_i = symbols('alpha_k r_t a_i d_i')
    theta_i, alpha_i = symbols('theta_i alpha_i')
    t_1, t_2, t_3 = symbols('theta_1 theta_2 theta_3')
    L_1, L_2, L_3 = symbols('L_1 L_2 L_3')
    x_b, y_b, z_b = symbols('z_b z_b z_b')
    x_p, y_p, z_p = symbols('x_p y_p z_p')

    # Rotation matrix (Roll -> Pitch -> Yaw).
    RotMatrRPY = rot_axis3(alpha).T*rot_axis2(beta).T*rot_axis1(gamma).T

    # Homogeneus transformation matrix body to global coordinates (B -> G).
    TransfMatrRPY = RotMatrRPY.col_insert(3, Matrix([x_b, y_b, z_b]))
    TransfMatrRPY = TransfMatrRPY.row_insert(3, Matrix([[0, 0, 0, 1]]))

    # Homogeneus translation / rotation of central body axis to each paw.
    # Also called "Local yaw".
    TransfMatrBP = Matrix([[cos(a_k), -sin(a_k), 0, r_t*cos(a_k)],
                           [sin(a_k), cos(a_k), 0, r_t*sin(a_k)],
                           [0, 0, 1, 0],
                           [0, 0, 0, 1]])

    # Homogeneus transformation matrix of link i to link i-1. 
    TransfMatrLink = Matrix([[cos(theta_i), -sin(theta_i)*cos(alpha_i),
                              sin(theta_i)*sin(alpha_i), a_i*cos(theta_i)],
                             [sin(theta_i), cos(theta_i)*cos(alpha_i),
                              -cos(theta_i)*sin(alpha_i), a_i*sin(theta_i)],
                             [0, sin(alpha_i), cos(alpha_i), d_i],
                             [0, 0, 0, 1]])
    
    # Transformation matrix for each paw.
    # Order: Link 1 -> 0, 2 -> 1, 3 -> 2
    TransfMatrL1 = TransfMatrLink.subs([(theta_i, t_1),
                                        (alpha_i, pi/2),
                                        (d_i, 0),
                                        (a_i, L_1)])
    TransfMatrL2 = TransfMatrLink.subs([(theta_i, t_2),
                                        (alpha_i, pi),
                                        (d_i, 0),
                                        (a_i, L_2)])
    TransfMatrL3 = TransfMatrLink.subs([(theta_i, t_3),
                                        (alpha_i, 0),
                                        (d_i, 0),
                                        (a_i, L_3)])

    # Transformation matrix from final paw joint to body (Link 0).
    TransfMatrPaw = simplify(TransfMatrL1*TransfMatrL2*TransfMatrL3)

    # Inverse Kinematics model
    # IK - Global to body (G -> B)
    TransfMatrRPY_IK = simplify(TransfMatrRPY**-1)

    # IK - Global to Paw (G -> 0)
    # Each paw has an end point coordinate denoted with x_p, y_p, z_p.
    TransfMatrPaw_IK_Coxa = [((TransfMatrRPY_IK
                             * TransfMatrBP**-1
                             * Matrix([x_p, y_p, z_p, 1]))
                             .subs([(a_k, (x+2)*pi/3),
                                   (L_1, geometry_values[0]),
                                   (L_2, geometry_values[1]),
                                   (L_3, geometry_values[2]),
                                   (r_t, geometry_values[3])])
                              ) for x in range(6)]
    TransfMatrPaw_IK_Femur = [((TransfMatrL1**-1
                              * TransfMatrPaw_IK_Coxa[x])
                              .subs(L_1, geometry_values[0])
                               ) for x in range(6)]

    TransfMatrPaw_IK_Coxa = [item.row_del(-1)
                             for item in TransfMatrPaw_IK_Coxa]
    TransfMatrPaw_IK_Femur = [item.row_del(-1)
                              for item in TransfMatrPaw_IK_Femur]
    return TransfMatrPaw_IK_Coxa, TransfMatrPaw_IK_Femur


# Documentation at http://effbot.org/tkinterbook/tkinter-index.htm
# To initialize Tkinter, we create a Tk root widget (an ordinary window).
root = Tk()
root.title('Hexapod Inverse Kinematics GUI')
root.configure(background='white')
# Frame widgets to split our window
# - Top: For inputs and display isometric hexapod plot.
# - Bottom: For the rest of hexapod plots.
topFrame = Frame(root)
topFrame.pack(fill=X)
bottomFrame = Frame(root)
bottomFrame.pack(fill=X, side=BOTTOM)

# Top frame split into another two frames.
leftTopFrame = Frame(topFrame)
leftTopFrame.pack(fill=X, side=LEFT)
rightTopFrame = Frame(topFrame)
rightTopFrame.pack(fill=X)


# rightTopFrame widgets
title_gui = Label(leftTopFrame, text="Hexapod isometric view",
                  font=("default", 14))

label_paw1 = Label(rightTopFrame, text='Paw N°1')
label_paw2 = Label(rightTopFrame, text='Paw N°2')
label_paw3 = Label(rightTopFrame, text='Paw N°3')
label_paw4 = Label(rightTopFrame, text='Paw N°4')
label_paw5 = Label(rightTopFrame, text='Paw N°5')
label_paw6 = Label(rightTopFrame, text='Paw N°6')


def set_scale_label(index, val):
    scaleValue[index].set('{}'.format(val))


x_paw1 = Scale(rightTopFrame, from_=0, to=10,
               showvalue=0)
x_paw1_val = Label(rightTopFrame, text='5')
y_paw1 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
y_paw1_val = Label(rightTopFrame, text='5')
z_paw1 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)

x_paw2 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
y_paw2 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
z_paw2 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)

x_paw3 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
y_paw3 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
z_paw3 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)

x_paw4 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
y_paw4 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
z_paw4 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)

x_paw5 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
y_paw5 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
z_paw5 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)

x_paw6 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
y_paw6 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)
z_paw6 = Scale(rightTopFrame, from_=0, to=10, showvalue=0)

alpha_angle = Scale(rightTopFrame, from_=0, to=45, orient=HORIZONTAL)
beta_angle = Scale(rightTopFrame, from_=0, to=45, orient=HORIZONTAL)
yaw_angle = Scale(rightTopFrame, from_=0, to=360, orient=HORIZONTAL)

# We call the pack method on this widget.
# This tells it to size itself to fit the given text, and make itself visible.
title_gui.grid(row=0)

label_paw1.grid(row=0, column=0, columnspan=3)
x_paw1.grid(row=1, column=0)
x_paw1_val.grid(row=2, column=0)
y_paw1.grid(row=1, column=1)
y_paw1_val.grid(row=2, column=1)
z_paw1.grid(row=1, column=2)


# We display the window using the Tkinter event loop.
# The program will stay in the event loop until we close the window
root.mainloop()

# eq_1, eq_2 = IK_Equations([38.5, 56.2, 143.5, 137.0])
