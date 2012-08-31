/*
 * Created by SharpDevelop.
 * User: thomas
 * Date: 28/03/2012
 * Time: 22:43
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace tbl_avr_gcc_installer
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.btn_cancel = new System.Windows.Forms.Button();
			this.grp_install = new System.Windows.Forms.GroupBox();
			this.rxt_console = new System.Windows.Forms.RichTextBox();
			this.pgb_installation = new System.Windows.Forms.ProgressBar();
			this.btn_create = new System.Windows.Forms.Button();
			this.grp_license = new System.Windows.Forms.GroupBox();
			this.btn_startInstall = new System.Windows.Forms.Button();
			this.chk_license = new System.Windows.Forms.CheckBox();
			this.rtxt_license = new System.Windows.Forms.RichTextBox();
			this.grp_welcome = new System.Windows.Forms.GroupBox();
			this.btn_start = new System.Windows.Forms.Button();
			this.lbl_welcome = new System.Windows.Forms.Label();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.grp_end = new System.Windows.Forms.GroupBox();
			this.btn_end = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.grp_install.SuspendLayout();
			this.grp_license.SuspendLayout();
			this.grp_welcome.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.grp_end.SuspendLayout();
			this.SuspendLayout();
			// 
			// btn_cancel
			// 
			this.btn_cancel.Location = new System.Drawing.Point(579, 276);
			this.btn_cancel.Name = "btn_cancel";
			this.btn_cancel.Size = new System.Drawing.Size(75, 23);
			this.btn_cancel.TabIndex = 0;
			this.btn_cancel.Text = "Cancel";
			this.btn_cancel.UseVisualStyleBackColor = true;
			this.btn_cancel.Click += new System.EventHandler(this.Button1Click);
			// 
			// grp_install
			// 
			this.grp_install.Controls.Add(this.rxt_console);
			this.grp_install.Controls.Add(this.pgb_installation);
			this.grp_install.Controls.Add(this.btn_cancel);
			this.grp_install.Location = new System.Drawing.Point(12, 110);
			this.grp_install.Name = "grp_install";
			this.grp_install.Size = new System.Drawing.Size(660, 317);
			this.grp_install.TabIndex = 1;
			this.grp_install.TabStop = false;
			this.grp_install.Text = "Setup in progress..";
			// 
			// rxt_console
			// 
			this.rxt_console.BackColor = System.Drawing.Color.Black;
			this.rxt_console.Enabled = false;
			this.rxt_console.ForeColor = System.Drawing.Color.Gainsboro;
			this.rxt_console.Location = new System.Drawing.Point(6, 20);
			this.rxt_console.Name = "rxt_console";
			this.rxt_console.ReadOnly = true;
			this.rxt_console.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.Vertical;
			this.rxt_console.Size = new System.Drawing.Size(648, 221);
			this.rxt_console.TabIndex = 1;
			this.rxt_console.Text = "";
			// 
			// pgb_installation
			// 
			this.pgb_installation.Location = new System.Drawing.Point(6, 247);
			this.pgb_installation.Name = "pgb_installation";
			this.pgb_installation.Size = new System.Drawing.Size(648, 23);
			this.pgb_installation.TabIndex = 0;
			// 
			// btn_create
			// 
			this.btn_create.Location = new System.Drawing.Point(487, 12);
			this.btn_create.Name = "btn_create";
			this.btn_create.Size = new System.Drawing.Size(154, 23);
			this.btn_create.TabIndex = 2;
			this.btn_create.Text = "Hidden: Create Repository";
			this.btn_create.UseVisualStyleBackColor = true;
			this.btn_create.Click += new System.EventHandler(this.Btn_createClick);
			// 
			// grp_license
			// 
			this.grp_license.Controls.Add(this.btn_startInstall);
			this.grp_license.Controls.Add(this.chk_license);
			this.grp_license.Controls.Add(this.rtxt_license);
			this.grp_license.Location = new System.Drawing.Point(12, 110);
			this.grp_license.Name = "grp_license";
			this.grp_license.Size = new System.Drawing.Size(660, 317);
			this.grp_license.TabIndex = 3;
			this.grp_license.TabStop = false;
			this.grp_license.Text = "Terms of Agreement";
			// 
			// btn_startInstall
			// 
			this.btn_startInstall.Enabled = false;
			this.btn_startInstall.Location = new System.Drawing.Point(536, 276);
			this.btn_startInstall.Name = "btn_startInstall";
			this.btn_startInstall.Size = new System.Drawing.Size(118, 23);
			this.btn_startInstall.TabIndex = 2;
			this.btn_startInstall.Text = "Start Installation";
			this.btn_startInstall.UseVisualStyleBackColor = true;
			this.btn_startInstall.Click += new System.EventHandler(this.Btn_startInstallClick);
			// 
			// chk_license
			// 
			this.chk_license.Location = new System.Drawing.Point(7, 245);
			this.chk_license.Name = "chk_license";
			this.chk_license.Size = new System.Drawing.Size(501, 25);
			this.chk_license.TabIndex = 1;
			this.chk_license.Text = "I agree this license which covers all files shipped with this installer.";
			this.chk_license.UseVisualStyleBackColor = true;
			this.chk_license.CheckedChanged += new System.EventHandler(this.Chk_licenseCheckedChanged);
			// 
			// rtxt_license
			// 
			this.rtxt_license.Location = new System.Drawing.Point(6, 20);
			this.rtxt_license.Name = "rtxt_license";
			this.rtxt_license.Size = new System.Drawing.Size(648, 221);
			this.rtxt_license.TabIndex = 0;
			this.rtxt_license.Text = "";
			// 
			// grp_welcome
			// 
			this.grp_welcome.Controls.Add(this.btn_start);
			this.grp_welcome.Controls.Add(this.lbl_welcome);
			this.grp_welcome.Location = new System.Drawing.Point(12, 110);
			this.grp_welcome.Name = "grp_welcome";
			this.grp_welcome.Size = new System.Drawing.Size(660, 317);
			this.grp_welcome.TabIndex = 4;
			this.grp_welcome.TabStop = false;
			this.grp_welcome.Text = "Welcome";
			// 
			// btn_start
			// 
			this.btn_start.Location = new System.Drawing.Point(566, 276);
			this.btn_start.Name = "btn_start";
			this.btn_start.Size = new System.Drawing.Size(75, 23);
			this.btn_start.TabIndex = 1;
			this.btn_start.Text = "Start";
			this.btn_start.UseVisualStyleBackColor = true;
			this.btn_start.Click += new System.EventHandler(this.Btn_startClick);
			// 
			// lbl_welcome
			// 
			this.lbl_welcome.Location = new System.Drawing.Point(91, 61);
			this.lbl_welcome.Name = "lbl_welcome";
			this.lbl_welcome.Size = new System.Drawing.Size(486, 209);
			this.lbl_welcome.TabIndex = 0;
			this.lbl_welcome.Text = resources.GetString("welcome");
			// 
			// pictureBox1
			// 
			this.pictureBox1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.BackgroundImage")));
			this.pictureBox1.InitialImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.InitialImage")));
			this.pictureBox1.Location = new System.Drawing.Point(12, 12);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(400, 90);
			this.pictureBox1.TabIndex = 5;
			this.pictureBox1.TabStop = false;
			// 
			// grp_end
			// 
			this.grp_end.Controls.Add(this.btn_end);
			this.grp_end.Controls.Add(this.label1);
			this.grp_end.Location = new System.Drawing.Point(12, 110);
			this.grp_end.Name = "grp_end";
			this.grp_end.Size = new System.Drawing.Size(660, 317);
			this.grp_end.TabIndex = 6;
			this.grp_end.TabStop = false;
			this.grp_end.Text = "Installation Complete";
			// 
			// btn_end
			// 
			this.btn_end.Location = new System.Drawing.Point(566, 276);
			this.btn_end.Name = "btn_end";
			this.btn_end.Size = new System.Drawing.Size(75, 23);
			this.btn_end.TabIndex = 1;
			this.btn_end.Text = "Done";
			this.btn_end.UseVisualStyleBackColor = true;
			this.btn_end.Click += new System.EventHandler(this.Btn_endClick);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(91, 61);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(486, 209);
			this.label1.TabIndex = 0;
			this.label1.Text = "Installation is complete.\r\n\r\nEnjoy";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(686, 447);
			this.Controls.Add(this.grp_end);
			this.Controls.Add(this.grp_welcome);
			this.Controls.Add(this.pictureBox1);
			this.Controls.Add(this.grp_license);
			this.Controls.Add(this.btn_create);
			this.Controls.Add(this.grp_install);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "MainForm";
			this.Text = "TBL AVR Library Installer";
			this.Load += new System.EventHandler(this.MainFormLoad);
			this.grp_install.ResumeLayout(false);
			this.grp_license.ResumeLayout(false);
			this.grp_welcome.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.grp_end.ResumeLayout(false);
			this.ResumeLayout(false);
		}
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btn_end;
		private System.Windows.Forms.GroupBox grp_end;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.Button btn_start;
		private System.Windows.Forms.Label lbl_welcome;
		private System.Windows.Forms.GroupBox grp_welcome;
		private System.Windows.Forms.Button btn_startInstall;
		private System.Windows.Forms.CheckBox chk_license;
		private System.Windows.Forms.RichTextBox rtxt_license;
		private System.Windows.Forms.GroupBox grp_license;
		private System.Windows.Forms.Button btn_create;
		private System.Windows.Forms.RichTextBox rxt_console;
		private System.Windows.Forms.ProgressBar pgb_installation;
		private System.Windows.Forms.GroupBox grp_install;
		private System.Windows.Forms.Button btn_cancel;
	}
}
