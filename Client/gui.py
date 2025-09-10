# gui.py
import tkinter as tk
from tkinter import messagebox
import database
import admin
import student
import ttkbootstrap as ttk  # Import ttkbootstrap for better UI

def login():
    username = entry_username.get()
    password = entry_password.get()

    role = database.get_user_role(username, password)

    if role:
        if role == "admin":
            admin.open_admin_dashboard()
        else:
            student.open_student_dashboard(username)
    else:
        messagebox.showerror("Login Failed", "Invalid credentials")

root = ttk.Window(themename="superhero")  # ttkbootstrap for modern UI
root.title("Exam System")

frame = ttk.Frame(root, padding=20)
frame.pack(pady=50)

ttk.Label(frame, text="Login", font=("Arial", 20)).pack()

ttk.Label(frame, text="Username:").pack()
entry_username = ttk.Entry(frame)
entry_username.pack()

ttk.Label(frame, text="Password:").pack()
entry_password = ttk.Entry(frame, show="*")
entry_password.pack()

ttk.Button(frame, text="Login", command=login, bootstyle="primary").pack(pady=10)

root.mainloop()
