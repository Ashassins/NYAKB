PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//12887481/958494/2.49/2/3/Schottky Diode

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r108.5_55"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.55) (shapeHeight 1.085))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "SOD2513X117N" (originalName "SOD2513X117N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r108.5_55) (pt -1.14, 0) (rotation 90))
			(pad (padNum 2) (padStyleRef r108.5_55) (pt 1.14, 0) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -1.975 1.15) (pt 1.975 1.15) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 1.975 1.15) (pt 1.975 -1.15) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 1.975 -1.15) (pt -1.975 -1.15) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -1.975 -1.15) (pt -1.975 1.15) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -0.83 0.64) (pt 0.83 0.64) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 0.83 0.64) (pt 0.83 -0.64) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 0.83 -0.64) (pt -0.83 -0.64) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -0.83 -0.64) (pt -0.83 0.64) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -0.83 0.098) (pt -0.288 0.64) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.682 0.64) (pt 0.83 0.64) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -0.83 -0.64) (pt 0.83 -0.64) (width 0.2))
		)
	)
	(symbolDef "BAT60JFILM" (originalName "BAT60JFILM")

		(pin (pinNum 1) (pt 100 mils 0 mils) (rotation 0) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 240 mils -15 mils) (rotation 0]) (justify "Left") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 700 mils 0 mils) (rotation 180) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 560 mils -15 mils) (rotation 0]) (justify "Right") (textStyleRef "Normal"))
		))
		(line (pt 300 mils 100 mils) (pt 300 mils -100 mils) (width 6 mils))
		(line (pt 300 mils 100 mils) (pt 340 mils 100 mils) (width 6 mils))
		(line (pt 340 mils 100 mils) (pt 340 mils 60 mils) (width 6 mils))
		(line (pt 300 mils -100 mils) (pt 260 mils -100 mils) (width 6 mils))
		(line (pt 260 mils -100 mils) (pt 260 mils -60 mils) (width 6 mils))
		(line (pt 200 mils 0 mils) (pt 300 mils 0 mils) (width 6 mils))
		(line (pt 500 mils 0 mils) (pt 600 mils 0 mils) (width 6 mils))
		(poly (pt 300 mils 0 mils) (pt 500 mils 100 mils) (pt 500 mils -100 mils) (pt 300 mils 0 mils) (width 10  mils))
		(attr "RefDes" "RefDes" (pt 500 mils 350 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 500 mils 250 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "BAT60JFILM" (originalName "BAT60JFILM") (compHeader (numPins 2) (numParts 1) (refDesPrefix D)
		)
		(compPin "1" (pinName "K") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "A") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "BAT60JFILM"))
		(attachedPattern (patternNum 1) (patternName "SOD2513X117N")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Manufacturer_Name" "STMicroelectronics")
		(attr "Manufacturer_Part_Number" "BAT60JFILM")
		(attr "Mouser Part Number" "511-BAT60JFILM")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/STMicroelectronics/BAT60JFILM?qs=GnFZssByvVVVnMaukcyLZg%3D%3D")
		(attr "Arrow Part Number" "BAT60JFILM")
		(attr "Arrow Price/Stock" "https://www.arrow.com/en/products/bat60jfilm/stmicroelectronics?region=nac")
		(attr "Mouser Testing Part Number" "")
		(attr "Mouser Testing Price/Stock" "")
		(attr "Description" "SMALL SIGNAL SCHOTTKY DIODE")
		(attr "<Hyperlink>" "https://www.mouser.jp/datasheet/2/389/cd00001944-1795661.pdf")
		(attr "<Component Height>" "1.17")
		(attr "<STEP Filename>" "BAT60JFILM.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=0")
	)

)