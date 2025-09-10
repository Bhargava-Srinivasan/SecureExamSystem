import tkinter as tk
from tkinter import messagebox
import database
from ttkbootstrap import Style
import ttkbootstrap as ttk  

def open_admin_dashboard():
    admin_window = tk.Toplevel()
    admin_window.title("Admin Dashboard")

    tk.Label(admin_window, text="Admin Panel", font=("Arial", 16)).pack(pady=10)

    # Section: Add Exam
    def add_exam_action():
        question = entry_question.get()
        answer = entry_answer.get()
        if question and answer:
            database.add_exam(question, answer)
            messagebox.showinfo("Success", "Exam added successfully")
            refresh_exams_list()  # Refresh the list after adding a new test

    tk.Label(admin_window, text="Question:").pack()
    entry_question = tk.Entry(admin_window, width=50)
    entry_question.pack()
    
    tk.Label(admin_window, text="Answer:").pack()
    entry_answer = tk.Entry(admin_window, width=50)
    entry_answer.pack()
    
    tk.Button(admin_window, text="Add Exam", command=add_exam_action).pack(pady=10)

    # Section: Delete Exam
    tk.Label(admin_window, text="Delete Exam", font=("Arial", 12, "bold")).pack(pady=10)
    
    exams_listbox = tk.Listbox(admin_window, width=50)
    exams_listbox.pack()

    def refresh_exams_list():
        """Refreshes the exam list in the Listbox"""
        exams_listbox.delete(0, tk.END)
        exams = database.get_all_exams()
        for exam in exams:
            exams_listbox.insert(tk.END, f"{exam[0]} - {exam[1]}")  # Display ID and question

    def delete_selected_exam():
        """Deletes the selected test from the listbox."""
        selected_exam = exams_listbox.get(tk.ACTIVE)
        if selected_exam:
            exam_id = selected_exam.split(" - ")[0]  # Extract exam ID
            database.delete_exam(exam_id)
            messagebox.showinfo("Success", "Exam deleted successfully")
            refresh_exams_list()  # Refresh list after deletion

    # Section: Add Student
    tk.Label(admin_window, text="Add Student", font=("Arial", 12, "bold")).pack(pady=10)

    tk.Label(admin_window, text="Username:").pack()
    entry_student_username = tk.Entry(admin_window, width=30)
    entry_student_username.pack()

    tk.Label(admin_window, text="Password:").pack()
    entry_student_password = tk.Entry(admin_window, width=30, show="*")
    entry_student_password.pack()

    def add_student_action():
        username = entry_student_username.get()
        password = entry_student_password.get()
        if username and password:
            database.add_student(username, password)
            messagebox.showinfo("Success", f"Student {username} added successfully!")
        else:
            messagebox.showerror("Error", "Please fill in both fields.")

    tk.Button(admin_window, text="Add Student", command=add_student_action).pack(pady=5)

    refresh_exams_list()
    tk.Button(admin_window, text="Delete Selected Exam", command=delete_selected_exam).pack(pady=5)
