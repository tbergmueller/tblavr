/*
 * Created by SharpDevelop.
 * User: tbergmueller
 * Date: 08.01.2012
 * Time: 22:58
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Diagnostics;
using System.IO;
using System.Collections;

using System.Text.RegularExpressions;

namespace texFromBin
{
	
	/// <summary>
	/// Replaces a defined Symbol (see Remarks) through content belonging to passed libFile (e.g. libtbl.a) 
	/// </summary>
	class TBLAvrPrototypeParser
	{
		string file_libfile;
		string file_dcsHeader;
		string file_texFile;
		string dir_lib;
		string initfunc_param = "NOTSET";
				
		static string commentSymbolPrototypelistPattern = WildcardToRegex("{dcs_prototype_list}");
		static string commentSymbolPrototypelistEndPattern = WildcardToRegex("{/dcs_prototype_list}");
		static string commentSymbolSignatureWildcard = "void DCS_BASIC_INIT(*);"; // will be replaced
		static string commentSymbolSignaturePattern = WildcardToRegex(commentSymbolSignatureWildcard);	// Prototype of compileable hardwarefunction
		
		static string filenamePatternSingle = WildcardToRegex("dcs_*_basic.o");
		static string filenamePatternSingleRw = WildcardToRegex("dcs_*_rw_basic.o");
		static string filenamePatternMulti = WildcardToRegex("dcs_*_usart*_basic.o");
		static string filenamePatternMultiRw = WildcardToRegex("dcs_*_usart*_rw_basic.o");
		
		// most complicated First, otherwise fehler bei pattern finden
		string[] filenamePatterns = {filenamePatternMultiRw, filenamePatternMulti, filenamePatternSingleRw, filenamePatternSingle};
		
		static ArrayList prototypes;
		static ArrayList filenames;
		
		// add in c: 			void DCS_BASIC_INIT(arguments); (will be replaced by init-function List)
		// add in latex:		%{dcs_prototype_list}
		//	 compileable line with symbols processor usart initfunction
		// e.g. 
		/*
		 * %{dcs_prototype_list}
		 * %processor & usart & initfunction
		 * .....
		 * %{/dcs_prototype_list}
		 */
		
		
		
		
		
		public TBLAvrPrototypeParser(string libFile, string libDir, string devComSlaveHeader, string texFile)
		{
			file_libfile = libFile;
			dir_lib = libDir;
			file_dcsHeader=devComSlaveHeader;
			file_texFile = texFile;			
			prototypes = new ArrayList();
			filenames = new ArrayList();
		}
		
		
		
		private bool parse()
		{
			foreach(string filename in filenames)
			{
				string func = parseFilenameToInitfunction(filename);
				prototypes.Add(func);
				Console.WriteLine(func);
				
			}
			
			return(true);
			
		}
		
		string[] headerfile; // holds values of original header file
		string[] texfile; // holds values of original tex-File
		int initifunc_line;
		
		int functable_line;
		int functable_end_line;
		string functable_pattern;
		
		string initfunc_tabs;
		
		private bool getHeaderFileIn()
		{		
			bool retVal = false;
			
			try
			{
				string[] hFile = System.IO.File.ReadAllLines(file_dcsHeader);
				
				int cnt=0;
				foreach(string s in hFile)
				{
					
					MatchCollection m = Regex.Matches(s, commentSymbolSignaturePattern);
					
					if(m.Count > 0)
					{
						initifunc_line = cnt;
						Console.WriteLine("Prototype of Initfunction found, line "+cnt.ToString()+" : " +s);
						 
						 initfunc_tabs = s.Split('v')[0]; // split by void
						 initfunc_param = s.Trim();
						 
						 string[] patternSplit = commentSymbolSignatureWildcard.Split('*');
						 
						 foreach(string part in patternSplit)
						 {
						 	initfunc_param = initfunc_param.Replace(part,"");
						 }
						 						 
						 Console.Write("Param extracted: " + initfunc_param);
						 retVal = true;
					}
					cnt++;
				}
				
				headerfile = hFile;
				return(retVal);
			}
			catch
			{
				Console.WriteLine("Fehler beim Lesen oder Schreiben der Headerdatei " + file_dcsHeader + " Execution aborted...");
				Program.Terminate(-1);
			}
			
			return(retVal);
		}
				
		private bool getTexFileIn()
		{		
			
			bool beginPatternFound = false;
			bool endPatternFound = false;
			
			try
			{
				string[] tFile = System.IO.File.ReadAllLines(file_texFile);
				
				int cnt=0;
				foreach(string s in tFile)
				{
					
					MatchCollection m = Regex.Matches(s, commentSymbolPrototypelistPattern);
					
					if(m.Count > 0)
					{
						functable_line = cnt;
						Console.WriteLine("Function Table Symbol found, line "+cnt.ToString()+" : " +s);
						 
						
						functable_pattern = tFile[cnt+1].Replace("%","");
						 
						 						 
						 Console.Write("Pattern extracted: " + functable_pattern);
						 beginPatternFound = true;
					}
					
					
					
					m = Regex.Matches(s, commentSymbolPrototypelistEndPattern);
					
					if(m.Count > 0)
					{
						
						Console.WriteLine("Function Table End Symbol found, line "+cnt.ToString()+" : " +s);
						functable_end_line = cnt;
												 
						 endPatternFound = true;
					}
					cnt++;
				}
				
				texfile = tFile;
				
				if(functable_line > functable_end_line)
				{
					return(false);
				}
				return(beginPatternFound && endPatternFound);
			}
			catch
			{
				Console.WriteLine("Fehler beim Lesen der Texdatei " + file_texFile + " Execution aborted...");
				Program.Terminate(-1);
			}
			
			return(beginPatternFound && endPatternFound);
		}
				
		private bool getInitFunctionsFromFiles()
		{			
			// Check file
			string currentfile = "test";
			
			string[] dirsToParse = Directory.GetDirectories(dir_lib);
			
			
			
			foreach(string dir in dirsToParse)
			{
				currentfile = dir;
				currentfile = Path.Combine(currentfile, file_libfile);
				if(!File.Exists(currentfile))
				{
					Console.WriteLine("ERROR: libfile " + currentfile + " does not exist. Aborted...");
					Program.Terminate(-1);
				}
				
				prototypes = new ArrayList();
				ProcessStartInfo startInfo = new ProcessStartInfo("objdump.exe");
				startInfo.WorkingDirectory = Environment.CurrentDirectory;
				startInfo.Arguments = "-G " + currentfile;
				startInfo.RedirectStandardOutput=true;
				startInfo.UseShellExecute=false;
				Process pro = new Process();
				pro.StartInfo = startInfo;
				
				pro.Start();
				pro.WaitForExit();
				
				// Read linewise
				
				
				int cnt = 0;
				
				while(!pro.StandardOutput.EndOfStream)
				{
					string line= pro.StandardOutput.ReadLine();
					
					// start with most specific ones
					bool ignore = false;
					
					foreach(string pattern in filenamePatterns)
					{					
						if(!ignore)
						{
							MatchCollection m = Regex.Matches(line,pattern);
					
							for(int i=0; i<m.Count; i++)
							{
								ignore=true;
								filenames.Add(m[i].ToString());
								Console.WriteLine(m[i].ToString());
							}
						}				
					}
					cnt++;
					
				}
			}
			
			return(true);
		}
		
		private string parseFilenameToInitfunction(string filename)
		{
			try
			{
				return("void " + filename.Replace("basic.o", "init") + "(" + initfunc_param + ");");
			}
			catch
			{
				Console.WriteLine("Conversion aborted! Illegal Pattern");
				Program.Terminate(-1);
			}
			return("How did you get here?!");
			
		}
		
		public static string WildcardToRegex(string pattern)
		 {
		  return Regex.Escape(pattern).
		   Replace("\\*", ".*").
		   Replace("\\?", ".");
		 }

		private bool writeNewHeaderfile()
		{
			try
			{
				StreamWriter sr = new StreamWriter(file_dcsHeader);
				for(int cnt=0; cnt<headerfile.Length; cnt++)
				{
					if(cnt != initifunc_line)
					{
						sr.WriteLine(headerfile[cnt]);
					}
					else
					{
						foreach(string proto in prototypes)
						{
							sr.WriteLine(initfunc_tabs + proto);
						}
					}
				}
				
				sr.Flush();
				sr.Close();
				return(true);
			}
			catch
			{
				Console.WriteLine("Error Writing Outputfile " + file_dcsHeader + " ... Terminating ... ");
				Program.Terminate(-1);
				                  
			}
			
			
			return(false);
		}
				
		private bool writeNewTexfile()
		{
			try
			{
				StreamWriter sr = new StreamWriter(file_texFile);
				
				for(int cnt=0; cnt<texfile.Length; cnt++)
				{
					if(cnt < functable_line || cnt > functable_end_line)
					{
						sr.WriteLine(texfile[cnt]);
					}
					else
					{
						sr.WriteLine(texfile[functable_line]);
						sr.WriteLine(texfile[functable_line+1]);
						
						foreach(string proto in prototypes)
						{
							string[] parts = proto.Split('_');
							string toWrite = functable_pattern;
							
							toWrite = toWrite.Replace("processor", parts[1].ToUpper());
							
							
							if(parts[2].Contains("usart"))
							{
								toWrite = toWrite.Replace("usart", parts[2].ToUpper());
							}
							else
							{
								toWrite = toWrite.Replace("usart", "USART");
							}	
							
							if(proto.Contains("_rw_"))
							{
								toWrite = toWrite.Replace("rw", "ja");
							}
							else
							{
								toWrite = toWrite.Replace("rw", "nein");
							}
							toWrite = toWrite.Replace("initfunction", proto.Split('(')[0]);

							sr.WriteLine(toWrite.Replace("_", "\\_"));
						}
						sr.WriteLine(texfile[functable_end_line]);
						cnt+=(functable_end_line-functable_line);
					}
				}
				
				sr.Flush();
				sr.Close();
				return(true);
			}
			catch
			{
				Console.WriteLine("Error Writing Outputfile " + file_dcsHeader + " ... Terminating ... ");
				Program.Terminate(-1);
				                  
			}
			
			
			return(false);
		}
		
		public bool ProcessFiles()
		{
			Console.WriteLine(Environment.NewLine + Environment.NewLine + "################################ Start Getting convertable Object filenames: ");
			getInitFunctionsFromFiles();
			Console.WriteLine(Environment.NewLine + Environment.NewLine + "################################ Reading in original Headerfile: ");
			if(!getHeaderFileIn())
			{
				Console.WriteLine("Error: Prototype that matches following Pattern could not be found: " + commentSymbolSignatureWildcard + " ... Program Terminating...");
				Program.Terminate(-1);
			}
			
			if(file_texFile != null)
			{
				Console.WriteLine(Environment.NewLine + Environment.NewLine + "################################ Reading in original TexFile: ");
				if(!getTexFileIn())
				{
					Console.WriteLine("Error: No (or not enough) Prototype-Table-Symbols are found: " + commentSymbolPrototypelistPattern + " or " + commentSymbolPrototypelistEndPattern + " ... Program Terminating...");
					Program.Terminate(-1);
				}
			}
			
			
			Console.WriteLine(Environment.NewLine +  Environment.NewLine + "################################ Start Conversion: ");
			parse();
			Console.WriteLine(Environment.NewLine +  Environment.NewLine + "################################ Start Writing headerFile: ");
			if(!writeNewHeaderfile())
			{
				Console.WriteLine("Error: Writing of new Headerfile unsuccessful: " + file_dcsHeader + " ... Program Terminating...");
				Program.Terminate(-1);
			}
			else
			{
				Console.WriteLine("successful");
			}
			
			if(file_texFile != null)
			{
				Console.WriteLine(Environment.NewLine +  Environment.NewLine + "################################ Start Writing Tex-File: ");
				if(!writeNewTexfile())
				{
					Console.WriteLine("Error: Writing of new Texfile unsuccessful: " + file_texFile + " ... Program Terminating...");
					Program.Terminate(-1);
				}
				else
				{
					Console.WriteLine("successful");
				}
			}
			return(true);
		}
	}
	
	class Program
	{
		
		public static void Terminate(int code)
		{
			#if DEBUG
				Console.ReadKey(true);
			#endif
			Environment.Exit(code);
		}
		
		static string rootDir="../../";
		static string ucLib=Path.Combine(rootDir, "avr/lib/");
		static string libname = "libtbl.a";
		static string texFile = Path.Combine(rootDir, "documents/modules/devcom/slave/implementations.tex");
		static string headerFile = Path.Combine(rootDir, "avr/include/tbl/DevComSlave.h");
				
		public static void Main(string[] args)
		{
			Console.WriteLine("Provided Argments: " + args.Length.ToString());
			
			headerFile = args[0];
			ucLib = args[1];
			
			
			if(args.Length == 3)
			{
				texFile = args[2];
			}
			else
			{
				texFile = null;
			}
			
			Console.WriteLine("Header File:" + args[0]);
			Console.WriteLine("Lib Directory: " + args[1]);
			Console.WriteLine("Lib name: " + libname);
			
						
			string path = System.Windows.Forms.Application.StartupPath;
			Environment.CurrentDirectory = path;
			
			TBLAvrPrototypeParser parser = new TBLAvrPrototypeParser(libname, ucLib, headerFile, texFile);
			
		    parser.ProcessFiles();
		    Console.WriteLine("Successfully finished..");
		    Terminate(0);
		}
	}
}