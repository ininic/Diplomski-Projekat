   M S B A M L   `   `   `  ����F  BProjekat_UI, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null)clr-namespace:Projekat_UIProjekat_UI  5   	       Projekat_UI.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   +localclr-namespace:Projekat_UI     5   	     � Title$  Simulacija RIP protokola��5   	   $	��635��6*   $��1268.667��67    � UseLayoutRounding . .��5
      � -   5        $
��grid��5
      $��	0,10,25,5q�6   .+�5   
   �� -   5        $��generate_btn��5      $��14(�6   $��Generiši rutere��64   $
��Left=�6N   $��
297,74,0,0q�6i   $	��Top=�6}   $	��120��6�   $��23��6�   6�   �� 5   
   -   5        $��router_nbr_tb��5       $��14(�6   $
��Left=�66   $��23��6Q   $��
146,74,0,0q�6]    �� TextWrapping$
 Wrap=�6q   $	��top=�6�   $	��120��6�    J� RenderTransformOrigin$ -0.217,0.304&�6�   .��6�   ��5      O Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem 5   -   �� 6   6sys&clr-namespace:System;assembly=mscorlib     6-    �� 
TargetType" ��6    �� 	Resources 5         CueBannerBrush j� 
AlignmentX j� 
AlignmentY j� Stretch	 !� Visual��%�   &	        !� 5      $
 Left=�6=   $ Center=�6O   $
 None=�6c   	 5      �� 5   "   $��(najviše 15)��6(   
�������6?   5      5      
 �� Triggers
 5      P� 5       P� Property � �� �� Empty P� Value# Z 62   . �5      ��  �� Property � �� �� Value# [  6;   5      P� 5       � �� 62   Z� . �5      ��  � ��# [  6;   5      P� 5       �@J�IsKeyboardFocused . . �5      ��  � ��
 �����6;   5      5!      .��5"      5#      �� 5$   
   $��14(�6   $��Unesite broj rutera��6   $
��Left=�6<   $��
141,40,0,0q�6W   $	��Top=�6k   $	��129��6�   �� 5%   
   -   5        $��canvas��5%      $
��Left=�6    $	��312��6;   $��
34,179,0,0q�6H   $	��Top=�6\   $	��461��6t   �� 5&   
   $��571,43,5,46q�6   .@�5'      �� ��5(      �� 5)       �� Orientation$ 
Horizontal=�6!   .+�5*      �� $��14(�6$   Ruter69   5+      .��5-      �� $��0,0,0,-1q�6   .+�5.      �� $��Visible=�6#   $��Visible=�6K   .��5/      �� -   5         ��Name$ router_info_tb��5/   "   $��14(�6;   $	��342��6I   $��Podaci o ruteru:��6V    �� 
IsReadOnly . 6�   50      ]� 51      $��27��6    6/   ]� 52      $	��300��6    .+�53      �� -   5        $��14(�53   !   $��Center=�6/   $��32��6L   $	��144��6X   .��6v   Dodaj uređaj6�   54      55      56      �� 57      ��58      �� 59      $ 
Horizontal=�6!   .+�5:      �� $��14(�6$   
Simulacija6>   5;      .��5=      �� $��0,0,0,-1q�6   .+�5>      �� $��Visible=�6#   .��5?      �� -   5        $ simulation_tb��5?   0   $��14(�6"   $	��339��6H    . 6j   5@      ]� 5B      .+�5C      �� $��14(�6    $��10q�6.   .��6:   Izvorišna adresa:6M   �� 5D      -   5        $��	source_tb��5D   "   $	��120��64   $��10q�6@   $��32��6L   $��15(�6X   $��Center=�6f   6�   �� 5E      $��14(�6    $��10q�6/   .��6;   Odredišna adresa:6N   �� 5F      -	   5        $��destination_tb��5F   "   $	��120��69   $��10q�6E   $��32��6Q   $��15(�6]   $��Center=�6k   6�   5G      ]� 5H      $	��300��6    .+�5I      �� -
   5        $��simulation_btn��5I   ;   $��14(�6!   $��10q�6/   $��Center=�6R   $��37��6o   $	��144��6{   .��6�   Pokreni simulaciju6�   5J      5K      5L      5M      	� 5N   
   -   5        $��	radio_rip��5N   '    	� 	GroupName$
 cost��6   $��Standardni RIP��68   $
��Left=�6Q   $��146,116,0,0q�6l   $	��Top=�6�   6�   	� 5O   
   -   5        $��
radio_ospf��5O   (   $
 cost��6   $��Prošireni RIP (OSPF)��6:   $
��Left=�6Y   $��297,116,0,0q�6t   $	��Top=�6�   6�   5P      5Q      