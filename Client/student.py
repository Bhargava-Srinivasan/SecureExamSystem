import tkinter as tk
from tkinter import messagebox
import database
from ttkbootstrap import Style
import ttkbootstrap as ttk 

def open_student_dashboard(username):
    student_window = tk.Toplevel()
    student_window.title("Student Dashboard")

    tk.Label(student_window, text=f"Welcome, {username}", font=("Arial", 16)).pack(pady=10)

    exams = database.get_all_exams()

    answer_entries = []

    for exam in exams:
        exam_id, question = exam
        tk.Label(student_window, text=question).pack()
        entry_answer = tk.Entry(student_window, width=50)
        entry_answer.pack()
        answer_entries.append((exam_id, entry_answer))

    def submit_all_answers():
        results = []  # To store evaluation results per question
        for exam_id, entry in answer_entries:
            answer = entry.get().strip()
            if answer:
                database.submit_answer(username, exam_id, answer)

                # Evaluate immediately after submitting
                is_correct = database.evaluate_submission(username, exam_id)
                results.append((exam_id, is_correct))

        # Prepare feedback message
        correct_count = sum(1 for _, correct in results if correct)
        total = len(results)
        messagebox.showinfo("Evaluation Complete", f"You got {correct_count} out of {total} correct.")


    tk.Button(student_window, text="Submit All Answers", command=submit_all_answers).pack(pady=10)
