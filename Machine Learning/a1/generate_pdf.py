import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.spatial.distance import pdist
from sklearn.datasets import load_iris

from reportlab.lib.pagesizes import letter
from reportlab.lib import colors
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, Image, HRFlowable, PageBreak
)
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.pdfgen import canvas

# ---------------------------------------------------------
# Step 1: Ensure plots are generated & saved
# ---------------------------------------------------------
work_dir = '/Users/amankashyap/Documents/Semester-5/Machine Learning'

# Question 1 Computation & Plot
dimensions = [2**i for i in range(11)]
num_points = 100
l2_sq_means, l2_sq_stds = [], []
l1_means, l1_stds = [], []

np.random.seed(42)
for d in dimensions:
    points = np.random.rand(num_points, d)
    l2_sq_dists = pdist(points, metric='sqeuclidean')
    l1_dists = pdist(points, metric='cityblock')
    
    l2_sq_means.append(np.mean(l2_sq_dists))
    l2_sq_stds.append(np.std(l2_sq_dists))
    l1_means.append(np.mean(l1_dists))
    l1_stds.append(np.std(l1_dists))

# Generate Q1 plot
fig, axes = plt.subplots(2, 2, figsize=(11, 7.5))
fig.suptitle('Question 1: Pairwise Distance Statistics as a Function of Dimension d', fontsize=14, fontweight='bold', y=0.98)

axes[0, 0].plot(dimensions, l2_sq_means, 'o-', color='#1f77b4', linewidth=2, label='Mean $L_2^2$')
axes[0, 0].set_xscale('log', base=2)
axes[0, 0].set_xlabel('Dimension $d$ (log scale)')
axes[0, 0].set_ylabel('Average Distance')
axes[0, 0].set_title('Average Squared Euclidean ($L_2^2$) Distance')
axes[0, 0].grid(True, ls="--", alpha=0.5)
axes[0, 0].legend()

axes[0, 1].plot(dimensions, l2_sq_stds, 's--', color='#d62728', linewidth=2, label='Std $L_2^2$')
axes[0, 1].set_xscale('log', base=2)
axes[0, 1].set_xlabel('Dimension $d$ (log scale)')
axes[0, 1].set_ylabel('Standard Deviation')
axes[0, 1].set_title('Standard Deviation of $L_2^2$ Distance')
axes[0, 1].grid(True, ls="--", alpha=0.5)
axes[0, 1].legend()

axes[1, 0].plot(dimensions, l1_means, 'o-', color='#2ca02c', linewidth=2, label='Mean $L_1$')
axes[1, 0].set_xscale('log', base=2)
axes[1, 0].set_xlabel('Dimension $d$ (log scale)')
axes[1, 0].set_ylabel('Average Distance')
axes[1, 0].set_title('Average $L_1$ Distance')
axes[1, 0].grid(True, ls="--", alpha=0.5)
axes[1, 0].legend()

axes[1, 1].plot(dimensions, l1_stds, 's--', color='#9467bd', linewidth=2, label='Std $L_1$')
axes[1, 1].set_xscale('log', base=2)
axes[1, 1].set_xlabel('Dimension $d$ (log scale)')
axes[1, 1].set_ylabel('Standard Deviation')
axes[1, 1].set_title('Standard Deviation of $L_1$ Distance')
axes[1, 1].grid(True, ls="--", alpha=0.5)
axes[1, 1].legend()

plt.tight_layout(rect=[0, 0, 1, 0.95])
q1_plot_path = os.path.join(work_dir, 'question1_plot.png')
plt.savefig(q1_plot_path, dpi=300)
plt.close()

# Question 2 Dataset & Visualizations
iris = load_iris()
df = pd.DataFrame(data=iris.data, columns=iris.feature_names)
df['target'] = iris.target
df['species'] = df['target'].map({i: name for i, name in enumerate(iris.target_names)})

# (a) Histograms
fig, axes = plt.subplots(2, 2, figsize=(10, 6.5))
fig.suptitle('Question 2(a): Feature Histograms of Iris Dataset', fontsize=14, fontweight='bold', y=0.98)
feature_colors = ['#1f77b4', '#2ca02c', '#ff7f0e', '#d62728']
for idx, feature in enumerate(iris.feature_names):
    ax = axes[idx // 2, idx % 2]
    df[feature].hist(ax=ax, bins=15, color=feature_colors[idx], edgecolor='black', alpha=0.75)
    ax.set_title(f'Histogram: {feature}', fontsize=11)
    ax.set_xlabel(feature)
    ax.set_ylabel('Frequency')
    ax.grid(True, linestyle='--', alpha=0.5)
plt.tight_layout(rect=[0, 0, 1, 0.95])
q2_hist_path = os.path.join(work_dir, 'question2_histograms.png')
plt.savefig(q2_hist_path, dpi=300)
plt.close()

# (b) Scatter & (c) Boxplot
fig, axes = plt.subplots(1, 2, figsize=(11, 5))
fig.suptitle('Question 2(b, c): Feature Scatter & Box Plots', fontsize=14, fontweight='bold', y=0.98)

# Scatter
ax_scatter = axes[0]
colors_map = {'setosa': '#d62728', 'versicolor': '#2ca02c', 'virginica': '#1f77b4'}
for species, group in df.groupby('species'):
    ax_scatter.scatter(group['petal length (cm)'], group['petal width (cm)'], 
                       label=species, color=colors_map[species], alpha=0.85, edgecolors='k', s=45)
ax_scatter.set_xlabel('Petal Length (cm)')
ax_scatter.set_ylabel('Petal Width (cm)')
ax_scatter.set_title('(b) Scatter: Petal Length vs Petal Width', fontsize=11)
ax_scatter.legend(title='Species')
ax_scatter.grid(True, linestyle='--', alpha=0.5)

# Boxplot
ax_box = axes[1]
df[iris.feature_names].boxplot(ax=ax_box, patch_artist=True, boxprops=dict(facecolor='#17becf', color='darkblue'))
ax_box.set_title('(c) Box Plot of All Features', fontsize=11)
ax_box.set_ylabel('Length / Width (cm)')
ax_box.set_xticklabels(['Sepal L', 'Sepal W', 'Petal L', 'Petal W'], rotation=15)
ax_box.grid(True, linestyle='--', alpha=0.5)

plt.tight_layout(rect=[0, 0, 1, 0.95])
q2_scatter_box_path = os.path.join(work_dir, 'question2_scatter_boxplot.png')
plt.savefig(q2_scatter_box_path, dpi=300)
plt.close()

# ---------------------------------------------------------
# Step 2: Build PDF Document using ReportLab
# ---------------------------------------------------------
class NumberedCanvas(canvas.Canvas):
    def __init__(self, *args, **kwargs):
        super(NumberedCanvas, self).__init__(*args, **kwargs)
        self._saved_page_states = []

    def showPage(self):
        self._saved_page_states.append(dict(self.__dict__))
        self._startPage()

    def save(self):
        num_pages = len(self._saved_page_states)
        for state in self._saved_page_states:
            self.__dict__.update(state)
            self.draw_page_decorations(num_pages)
            super(NumberedCanvas, self).showPage()
        super(NumberedCanvas, self).save()

    def draw_page_decorations(self, page_count):
        self.saveState()
        self.setFont("Helvetica", 9)
        self.setFillColor(colors.HexColor("#555555"))
        
        # Header (on pages after page 1)
        if self._pageNumber > 1:
            self.drawString(54, 750, "Machine Learning Lab 1 — Solutions & Writeup")
            self.setStrokeColor(colors.HexColor("#cccccc"))
            self.setLineWidth(0.5)
            self.line(54, 742, 558, 742)
            
        # Footer
        page_text = f"Page {self._pageNumber} of {page_count}"
        self.drawRightString(558, 36, page_text)
        self.drawString(54, 36, "Lab1_writeup.pdf | Department of Computer Science & Engineering")
        self.setStrokeColor(colors.HexColor("#cccccc"))
        self.setLineWidth(0.5)
        self.line(54, 48, 558, 48)
        
        self.restoreState()

pdf_path = os.path.join(work_dir, 'Lab1_writeup.pdf')
doc = SimpleDocTemplate(
    pdf_path,
    pagesize=letter,
    leftMargin=54,
    rightMargin=54,
    topMargin=60,
    bottomMargin=60
)

styles = getSampleStyleSheet()

# Custom styles
primary_color = colors.HexColor("#1A365D")   # Deep navy
secondary_color = colors.HexColor("#2B6CB0") # Slate blue
dark_text = colors.HexColor("#2D3748")       # Charcoal

title_style = ParagraphStyle(
    'DocTitle',
    parent=styles['Heading1'],
    fontName='Helvetica-Bold',
    fontSize=22,
    leading=26,
    textColor=primary_color,
    spaceAfter=4
)

subtitle_style = ParagraphStyle(
    'DocSubtitle',
    parent=styles['Normal'],
    fontName='Helvetica',
    fontSize=11,
    leading=14,
    textColor=colors.HexColor("#4A5568"),
    spaceAfter=15
)

h1_style = ParagraphStyle(
    'SectionH1',
    parent=styles['Heading1'],
    fontName='Helvetica-Bold',
    fontSize=14,
    leading=18,
    textColor=primary_color,
    spaceBefore=14,
    spaceAfter=8,
    keepWithNext=True
)

h2_style = ParagraphStyle(
    'SectionH2',
    parent=styles['Heading2'],
    fontName='Helvetica-Bold',
    fontSize=11,
    leading=15,
    textColor=secondary_color,
    spaceBefore=10,
    spaceAfter=6,
    keepWithNext=True
)

body_style = ParagraphStyle(
    'BodyDark',
    parent=styles['Normal'],
    fontName='Helvetica',
    fontSize=9.5,
    leading=13.5,
    textColor=dark_text,
    spaceAfter=6
)

bullet_style = ParagraphStyle(
    'BulletDark',
    parent=body_style,
    leftIndent=15,
    firstLineIndent=-10,
    spaceAfter=4
)

table_header_style = ParagraphStyle(
    'TableHeader',
    parent=styles['Normal'],
    fontName='Helvetica-Bold',
    fontSize=8.5,
    leading=11,
    textColor=colors.white,
    alignment=1
)

table_cell_style = ParagraphStyle(
    'TableCell',
    parent=styles['Normal'],
    fontName='Helvetica',
    fontSize=8,
    leading=10.5,
    textColor=dark_text,
    alignment=1
)

story = []

# Title & Metadata Header
story.append(Paragraph("IML-Lab -1 — Assignment Writeup", title_style))
story.append(Paragraph("<b>Course:</b> Machine Learning (Semester 5) &nbsp;|&nbsp; <b>Date:</b> August 3, 2026 &nbsp;|&nbsp; <b>Submission Output:</b> Lab1_writeup.pdf", subtitle_style))
story.append(HRFlowable(width="100%", thickness=1.5, color=primary_color, spaceBefore=0, spaceAfter=12))

# =========================================================
# QUESTION 1 SECTION
# =========================================================
story.append(Paragraph("Practice Question 1: Distance Dynamics & Curse of Dimensionality", h1_style))
story.append(Paragraph(
    "<b>Task Statement:</b> For each choice of dimension <i>d</i> &in; {2<sup>0</sup>, 2<sup>1</sup>, 2<sup>2</sup>, ..., 2<sup>10</sup>}, sample 100 points from the unit cube, and record the average distances between all pairs of points, as well as the standard deviation of the distances.",
    body_style
))

# Formulations box
story.append(Paragraph("<b>Mathematical Distance Metrics:</b>", h2_style))
math_text = """
i. <b>Squared Euclidean (<i>L</i><sub>2</sub><sup>2</sup>) distance:</b> &nbsp; ||<b>x</b> - <b>y</b>||<sub>2</sub><sup>2</sup> = &sum;<sub>j=1</sub><sup>d</sup> (x<sub>j</sub> - y<sub>j</sub>)<sup>2</sup><br/>
ii. <b><i>L</i><sub>1</sub> distance (Cityblock):</b> &nbsp; ||<b>x</b> - <b>y</b>||<sub>1</sub> = &sum;<sub>j=1</sub><sup>d</sup> |x<sub>j</sub> - y<sub>j</sub>|
"""
story.append(Paragraph(math_text, body_style))

story.append(Paragraph("<b>Recorded Statistics Table:</b>", h2_style))

# Build Q1 Table Data
q1_table_data = [[
    Paragraph("Dimension (<i>d</i>)", table_header_style),
    Paragraph("Mean <i>L</i><sub>2</sub><sup>2</sup>", table_header_style),
    Paragraph("Std <i>L</i><sub>2</sub><sup>2</sup>", table_header_style),
    Paragraph("Mean <i>L</i><sub>1</sub>", table_header_style),
    Paragraph("Std <i>L</i><sub>1</sub>", table_header_style)
]]

for idx, d_val in enumerate(dimensions):
    q1_table_data.append([
        Paragraph(str(d_val), table_cell_style),
        Paragraph(f"{l2_sq_means[idx]:.4f}", table_cell_style),
        Paragraph(f"{l2_sq_stds[idx]:.4f}", table_cell_style),
        Paragraph(f"{l1_means[idx]:.4f}", table_cell_style),
        Paragraph(f"{l1_stds[idx]:.4f}", table_cell_style)
    ])

q1_table = Table(q1_table_data, colWidths=[80, 100, 100, 100, 100])
q1_table.setStyle(TableStyle([
    ('BACKGROUND', (0, 0), (-1, 0), primary_color),
    ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
    ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
    ('GRID', (0, 0), (-1, -1), 0.5, colors.HexColor("#CBD5E0")),
    ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, colors.HexColor("#F7FAFC")]),
    ('TOPPADDING', (0, 0), (-1, -1), 3),
    ('BOTTOMPADDING', (0, 0), (-1, -1), 3),
]))
story.append(q1_table)
story.append(Spacer(1, 10))

# Embed Q1 Plot
story.append(Paragraph("<b>Distance & Standard Deviation Plots as a Function of <i>d</i>:</b>", h2_style))
story.append(Image(q1_plot_path, width=480, height=315))
story.append(Spacer(1, 8))

# Q1 Observations
story.append(Paragraph("<b>Observations about Distance and Standard Deviation:</b>", h2_style))
q1_obs = [
    "<b>Linear Growth of Average Distance:</b> Both mean <i>L</i><sub>2</sub><sup>2</sup> and mean <i>L</i><sub>1</sub> distances scale linearly with dimension <i>d</i> (e.g., E[<i>L</i><sub>2</sub><sup>2</sup>] = <i>d</i>/6 and E[<i>L</i><sub>1</sub>] = <i>d</i>/3).",
    "<b>Sub-linear Growth of Standard Deviation:</b> The standard deviation &sigma; of the pairwise distances grows proportionally to &radic;<i>d</i>.",
    "<b>Distance Concentration (Curse of Dimensionality):</b> Because the mean grows linearly with <i>d</i> while the standard deviation grows only as &radic;<i>d</i>, the relative variance (&sigma; / &mu;) approaches zero as <i>d</i> &rarr; &infin;. Consequently, in high-dimensional spaces (e.g. <i>d</i> = 1024), all pairwise distances concentrate around the mean value. Nearest-neighbor distances become nearly equal to farthest-neighbor distances, rendering distance metrics less discriminative."
]

for obs in q1_obs:
    story.append(Paragraph(f"&bull; {obs}", bullet_style))

story.append(Spacer(1, 14))
story.append(PageBreak())

# =========================================================
# QUESTION 2 SECTION
# =========================================================
story.append(Paragraph("Practice Question 2: Iris Dataset Workflow & EDA", h1_style))
story.append(Paragraph(
    "<b>Objective:</b> Understand the basic workflow of machine learning, including data loading, exploration, preprocessing, and visualization using the benchmark Iris Dataset.",
    body_style
))

# Tasks i - vi Summary
story.append(Paragraph("<b>Tasks i &ndash; vi: Environment, Dataset Overview & Preprocessing</b>", h2_style))
meta_text = """
i. <b>Libraries Imported:</b> NumPy, Pandas, Matplotlib, Scikit-learn.<br/>
ii. <b>Dataset Loaded:</b> Iris dataset from <code>sklearn.datasets</code>.<br/>
iii. <b>DataFrame Conversion:</b> Dataset converted to a Pandas DataFrame.<br/>
v. <b>Dataset Dimensions:</b> Number of samples = <b>150</b>, Number of features = <b>4</b>.<br/>
vi. <b>Missing Values Check:</b> <b>0 missing values</b> across all 4 features.
"""
story.append(Paragraph(meta_text, body_style))

# Task iv: First 5 rows table
story.append(Paragraph("<b>Task iv: Display First 5 Rows of Dataset</b>", h2_style))

q2_head_data = [[
    Paragraph("<b>Idx</b>", table_header_style),
    Paragraph("<b>Sepal L (cm)</b>", table_header_style),
    Paragraph("<b>Sepal W (cm)</b>", table_header_style),
    Paragraph("<b>Petal L (cm)</b>", table_header_style),
    Paragraph("<b>Petal W (cm)</b>", table_header_style),
    Paragraph("<b>Species</b>", table_header_style)
]]

for idx in range(5):
    row = df.iloc[idx]
    q2_head_data.append([
        Paragraph(str(idx), table_cell_style),
        Paragraph(f"{row['sepal length (cm)']:.1f}", table_cell_style),
        Paragraph(f"{row['sepal width (cm)']:.1f}", table_cell_style),
        Paragraph(f"{row['petal length (cm)']:.1f}", table_cell_style),
        Paragraph(f"{row['petal width (cm)']:.1f}", table_cell_style),
        Paragraph(str(row['species']), table_cell_style)
    ])

t_head = Table(q2_head_data, colWidths=[40, 90, 90, 90, 90, 80])
t_head.setStyle(TableStyle([
    ('BACKGROUND', (0, 0), (-1, 0), primary_color),
    ('GRID', (0, 0), (-1, -1), 0.5, colors.HexColor("#CBD5E0")),
    ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, colors.HexColor("#F7FAFC")]),
    ('TOPPADDING', (0, 0), (-1, -1), 3),
    ('BOTTOMPADDING', (0, 0), (-1, -1), 3),
]))
story.append(t_head)
story.append(Spacer(1, 8))

# Task vii: Summary Statistics Table
story.append(Paragraph("<b>Task vii: Statistical Measures for Each Feature</b>", h2_style))

features_df = df[iris.feature_names]
q2_stats_data = [[
    Paragraph("<b>Feature Name</b>", table_header_style),
    Paragraph("<b>Mean</b>", table_header_style),
    Paragraph("<b>Median</b>", table_header_style),
    Paragraph("<b>Min Value</b>", table_header_style),
    Paragraph("<b>Max Value</b>", table_header_style)
]]

for feat in iris.feature_names:
    q2_stats_data.append([
        Paragraph(feat, table_cell_style),
        Paragraph(f"{features_df[feat].mean():.4f}", table_cell_style),
        Paragraph(f"{features_df[feat].median():.4f}", table_cell_style),
        Paragraph(f"{features_df[feat].min():.4f}", table_cell_style),
        Paragraph(f"{features_df[feat].max():.4f}", table_cell_style)
    ])

t_stats = Table(q2_stats_data, colWidths=[160, 80, 80, 80, 80])
t_stats.setStyle(TableStyle([
    ('BACKGROUND', (0, 0), (-1, 0), secondary_color),
    ('GRID', (0, 0), (-1, -1), 0.5, colors.HexColor("#CBD5E0")),
    ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, colors.HexColor("#F7FAFC")]),
    ('TOPPADDING', (0, 0), (-1, -1), 3),
    ('BOTTOMPADDING', (0, 0), (-1, -1), 3),
]))
story.append(t_stats)
story.append(Spacer(1, 10))

# Task viii: Visualizations
story.append(Paragraph("<b>Task viii: Visualizations (a) Histogram, (b) Scatter Plot, (c) Box Plot</b>", h2_style))
story.append(Image(q2_hist_path, width=480, height=270))
story.append(Spacer(1, 6))
story.append(Image(q2_scatter_box_path, width=480, height=210))
story.append(Spacer(1, 8))

# Task ix: Three Key Observations
story.append(Paragraph("<b>Task ix: Three Key Observations</b>", h2_style))
q2_obs = [
    "<b>Linearly Separable Species:</b> Setosa is easily separable from Versicolor and Virginica based on petal length and width.",
    "<b>High Feature Correlation:</b> Petal length and petal width display a strong positive linear correlation.",
    "<b>Outlier Presence:</b> Sepal width exhibits mild outliers on both upper and lower tails, whereas other features show a clean continuous distribution without significant outliers."
]

for obs in q2_obs:
    story.append(Paragraph(f"1. {obs}" if "Linearly" in obs else (f"2. {obs}" if "High" in obs else f"3. {obs}"), bullet_style))

# Build Document
doc.build(story, canvasmaker=NumberedCanvas)
print(f"SUCCESS: PDF created at: {pdf_path}")
