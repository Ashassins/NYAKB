SamacSys ECAD Model
686646/958494/2.49/2/3/Diode

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r105_50"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 0.5) (shapeHeight 1.05))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "SOD2513X110N" (originalName "SOD2513X110N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r105_50) (pt -1.138, 0) (rotation 90))
			(pad (padNum 2) (padStyleRef r105_50) (pt 1.138, 0) (rotation 90))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 30)
			(line (pt -1.95 1.2) (pt 1.95 1.2) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 1.95 1.2) (pt 1.95 -1.2) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 1.95 -1.2) (pt -1.95 -1.2) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -1.95 -1.2) (pt -1.95 1.2) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -0.875 0.65) (pt 0.875 0.65) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 0.875 0.65) (pt 0.875 -0.65) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 0.875 -0.65) (pt -0.875 -0.65) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -0.875 -0.65) (pt -0.875 0.65) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -0.875 0.125) (pt -0.35 0.65) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt -1.662 0.65) (pt 0.875 0.65) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -0.875 -0.65) (pt 0.875 -0.65) (width 0.2))
		)
	)
	(symbolDef "BAT_60B_E6327" (originalName "BAT_60B_E6327")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 140 mils -15 mils) (rotation 0]) (justify "Left") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 600 mils 0 mils) (rotation 180) (pinLength 100 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 460 mils -15 mils) (rotation 0]) (justify "Right") (textStyleRef "Default"))
		))
		(line (pt 200 mils 100 mils) (pt 200 mils -100 mils) (width 6 mils))
		(line (pt 100 mils 0 mils) (pt 200 mils 0 mils) (width 6 mils))
		(line (pt 400 mils 0 mils) (pt 500 mils 0 mils) (width 6 mils))
		(poly (pt 200 mils 0 mils) (pt 400 mils 100 mils) (pt 400 mils -100 mils) (pt 200 mils 0 mils) (width 10  mils))
		(attr "RefDes" "RefDes" (pt 450 mils 200 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "BAT_60B_E6327" (originalName "BAT_60B_E6327") (compHeader (numPins 2) (numParts 1) (refDesPrefix D)
		)
		(compPin "1" (pinName "K") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "A") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "BAT_60B_E6327"))
		(attachedPattern (patternNum 1) (patternName "SOD2513X110N")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Mouser Part Number" "726-BAT60BE6327")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Infineon-Technologies/BAT-60B-E6327?qs=mzcOS1kGbgdKRNElyBqymA%3D%3D")
		(attr "Manufacturer_Name" "Infineon")
		(attr "Manufacturer_Part_Number" "BAT 60B E6327")
		(attr "Description" "Schottky Diodes & Rectifiers Silicon Schottky Diode")
		(attr "Datasheet Link" "")
		(attr "Height" "1.1 mm")
	)

)
