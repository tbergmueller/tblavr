/*
 * Created by SharpDevelop.
 * User: thomas
 * Date: 28/03/2012
 * Time: 22:43
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;

using System.Diagnostics;
using System.IO;
using TBL.EDOLL;
using ICSharpCode.SharpZipLib;

namespace tbl_avr_gcc_installer
{	
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			/*ArrayList test = new ArrayList();
			test.Add("test1");
			test.Add("Toolchain2");
			installTBLBinUtils(test);*/
			avrGccDistributions = new ArrayList();
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
			stdOut.DeactivateDebug();
			stdOut.SetInterface(StdOutInterfaces.WindowsForm);
		}
		
		
		private ArrayList avrGccDistributions;
		
		
		private void searchWinAvr()
		{
			// WinAVR is stored there	
			string envPath = Environment.GetEnvironmentVariable("Path"); 		
			string[] envPaths = envPath.Split(';');
			
			foreach(string s in envPaths)
			{
				if(s.Contains("WinAVR"))
				{
					if(!s.Contains("util"))
					{
						// fount
						String found = s.Replace("bin","");
						avrGccDistributions.Add(found);
						output("WinAVR-Distr. found: " + found);
					}
				}
			}
		}
		
		private bool installationHasErrors = false;
		
		
		void DirSearch(string dir, string searchedDir)
		{
			try
			{
				foreach(string d in Directory.GetDirectories(dir))
				{
					if(d.Contains(searchedDir))
					{
						output("Atmel AVRToolchain found: " + d);
						avrGccDistributions.Add(d);
					}
					else
					{
						DirSearch(d, searchedDir);
					}
				}
			}
			catch(Exception e)
			{
				Console.WriteLine(e.Message);
			}
			
		}
		
		private static string tempDirectory = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "libtblInstall" + System.IO.Path.DirectorySeparatorChar);
		private static string includeZip = Path.Combine(TBL.Runtime.ExecutableDirectory, "include.zip");
		private static string libZip = Path.Combine(TBL.Runtime.ExecutableDirectory, "lib.zip");
		
		private static string includeDir = "include" + System.IO.Path.DirectorySeparatorChar;
		private static string libDir = "lib" + System.IO.Path.DirectorySeparatorChar;
		
		private static string passwd = "thomasRul3z";
		
		private bool extractData()
		{
			FolderZipper.ZipUtil.UnZipFiles(includeZip, Path.Combine(tempDirectory, includeDir), passwd, false);
			FolderZipper.ZipUtil.UnZipFiles(libZip, Path.Combine(tempDirectory, libDir), passwd, false);
			
			output("Data Extraction completed to " + tempDirectory);
			return true;
		}
		
		private static string FILENAME_VERSIONINFO = "version.txt";
		private static string FILENAME_INSTALLATION_INFO="installedVersions.txt";
		private static string FOLDERNAME_BINUTILS="TBLUtils";
		
		private bool installTBLBinUtils(ArrayList modifiedToolchains)
		{
			TblAvrVersionInfo versionInfo = TblAvrVersionInfo.GetFromFile(FILENAME_VERSIONINFO);
			
			string installationFolder = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86),FOLDERNAME_BINUTILS);
			
			string installedInfo = Path.Combine(installationFolder,FILENAME_INSTALLATION_INFO);
			try
			{
				Directory.CreateDirectory(installationFolder);
			}
			catch(Exception e)
			{
				// TODO
			}
			
			StreamWriter sw = new StreamWriter(installedInfo);
			
			foreach(string tc in modifiedToolchains)
			{
				sw.WriteLine(tc + ";" + versionInfo.VersionNumber);
			}
			
			sw.Flush();
			sw.Close();
			
			
			
			return(false);
		}
		
		private bool compressData()
		{
			FolderZipper.ZipUtil.ZipFiles(Path.Combine(TBL.Runtime.ExecutableDirectory, includeDir),includeZip, passwd, true);
			FolderZipper.ZipUtil.ZipFiles(Path.Combine(TBL.Runtime.ExecutableDirectory, libDir), libZip, passwd, true);
			
			return true;
		}
		
		private void searchAvrToolchain()
		{
			ArrayList dirsToSearch = new ArrayList();
			
			
			dirsToSearch.Add(Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFiles));
			dirsToSearch.Add(Environment.GetFolderPath(Environment.SpecialFolder.CommonProgramFilesX86));
			dirsToSearch.Add(Environment.GetFolderPath(Environment.SpecialFolder.CommonPrograms));
			dirsToSearch.Add(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles));
			dirsToSearch.Add(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86));
			dirsToSearch.Add(Environment.GetFolderPath(Environment.SpecialFolder.Programs)); 
			
			//dirsToSearch.Add("C:\\Program Files (x86)\\Atmel");
			
			
			foreach(string rootdir in dirsToSearch)
			{
				DirSearch(rootdir, "AVRToolchain");
			}
			
		}
		
		
		private bool searchDistributions()
		{
			avrGccDistributions.Clear();
			
			searchWinAvr();			
			
			searchAvrToolchain();
			
			if(avrGccDistributions.Count > 0)
			{
				
				return(true);
			}
			return(false);
		}
		
		void output(String text)
		{
			rxt_console.Text += "> " + text + Environment.NewLine;
			this.Refresh();
		}
		
		
		string execShellCommand(string vApplication, string vArguments)
		{
			Process p = new Process();
			p.StartInfo.UseShellExecute = false;
			p.StartInfo.RedirectStandardOutput = true;
			
			p.StartInfo.Arguments = vArguments;
			p.StartInfo.FileName = vApplication;
			
			p.Start();
			
			string output = p.StandardOutput.ReadToEnd();
			
			p.WaitForExit();
			
			return(output);
		}
		
		private void progress(int percentage)
		{
			pgb_installation.Value = percentage;
			this.Refresh();
		}
		
		
		bool install()
		{			
			grp_install.Visible = true;	
			grp_install.Enabled = true;
			rxt_console.Enabled = true;			
			
			installationHasErrors = false;
			
			output("#### Extracting Data ####");
			extractData();
			progress(5);
			
			output("### Searching WinAVR/AVR-Toolchains - these may take some minutes depending on your system size... ###");
			progress(15);
			
			if(!this.searchDistributions())
			{
				installationHasErrors = true;
				output("ERROR: No Toolchains found to update...");
				return false;
				//MessageBox.Show("Distributions Found");
			}
			else
			{
				// TODO: Let the user select which ones to install...
			}
			progress(30);
			
			output("### Update Toolchains ###");
			
			foreach(string toolchain in avrGccDistributions)
			{
				copyFilesToDistributions(toolchain);
			}
			
			progress(90);
			output("###### Delete temporary Files... ########");
			Directory.Delete(tempDirectory, true);
			installTBLBinUtils(avrGccDistributions);
			progress(100);
			
			if(installationHasErrors)
			{
				stdOut.Error("Installation has errors. Please refer log window for details." + Environment.NewLine + Environment.NewLine + "Make sure all files belonging to TBL are closed and no IDEs (Atmel Studio, AVR-Studio, Eclipse, ..) are openened." + Environment.NewLine + Environment.NewLine + "Restart installer then...");
			}
			else
			{
				hideGroupboxes();
				grp_end.Visible = true;
			}

			
			
			return(true);
		}
		
		bool copyFilesToDistributions(string distribution)
		{
			string tmpPath = Path.Combine(distribution, @"lib\gcc\avr"); // Path to folder where version numbers are in
			
			if(!Directory.Exists(tmpPath))
			{
				installationHasErrors = true;
				output("ERROR: Directory doesn't exist: " + tmpPath);
				return(false);
			}
			string[] dirs = Directory.GetDirectories(tmpPath);
			
				
			foreach(string dir in dirs) // for all versions
			{
				string installDir = Path.Combine(tmpPath, dir);
				output("Install Libraries to: " + installDir + "... ");
				
				#region Install Libraries
				string[] srcDirs = Directory.GetDirectories(Path.Combine(tempDirectory, libDir));
				
				foreach(string srcFolder in srcDirs) // For all Folders in Repository....
				{					
					foreach(string srcFilename in Directory.GetFiles(srcFolder)) // All Sourcefilenames...
					{
						string targetFilename = Path.Combine(installDir, srcFilename.Replace(Path.Combine(tempDirectory, libDir), ""));
												
						try
						{
							File.Copy(srcFilename, targetFilename, true); // Copy an doverwrite
						}
						catch
						{
							installationHasErrors = true;
							output("ERROR: Could not copy file to " + targetFilename);
						}
						                             
					}					
				}
				#endregion
				
				output("Install headers to: " + Path.Combine(installDir, includeDir));
				
				#region install headers
				
				srcDirs = Directory.GetDirectories(Path.Combine(tempDirectory, includeDir));				
				
				foreach(string srcFolder in srcDirs) // For all Folders in Repository....
				{		
					string incFolder = srcFolder.Replace(Path.Combine(tempDirectory, includeDir), "");
					string targetDir = Path.Combine( Path.Combine(installDir, includeDir), incFolder);
					
					if(!Directory.Exists(targetDir))
					{
						Directory.CreateDirectory(targetDir);
					}					
					
					foreach(string srcFilename in Directory.GetFiles(srcFolder)) // All Sourcefilenames...
					{
						string targetFilename = Path.Combine( Path.Combine(installDir, includeDir), srcFilename.Replace(Path.Combine(tempDirectory, includeDir), ""));
												
						try
						{
							File.Copy(srcFilename, targetFilename, true); // Copy an doverwrite
						}
						catch
						{
							installationHasErrors = true;
							output("ERROR: Could not copy file to " + targetFilename);
						}
						                             
					}	
				}
				#endregion				
			}
			
			return(true);
		}
				
		void Button1Click(object sender, EventArgs e)
		{
			// cancel installation
			MessageBox.Show("Currently not implemented... sorry dude ;)");
		}
		
		void MainFormLoad(object sender, EventArgs e)
		{
			hideGroupboxes();
			grp_welcome.Visible = true;
			rtxt_license.Text = new System.IO.StreamReader("license.txt").ReadToEnd();
		}
		
		void Btn_createClick(object sender, EventArgs e)
		{
			compressData();
		}
		
		void Chk_licenseCheckedChanged(object sender, EventArgs e)
		{
			btn_startInstall.Enabled = chk_license.Checked;
		}
		
		void Btn_startInstallClick(object sender, EventArgs e)
		{
			hideGroupboxes();
			this.install();
		}
		
		void hideGroupboxes()
		{
			grp_license.Visible = false;
			grp_welcome.Visible = false;
			grp_install.Visible = false;
			grp_end.Visible = false;
		}
		
		void Btn_startClick(object sender, EventArgs e)
		{
			hideGroupboxes();
			grp_license.Visible = true;
		}
		
		void Btn_endClick(object sender, EventArgs e)
		{
			Environment.Exit(0);
		}
	}
}
