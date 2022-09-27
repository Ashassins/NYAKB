SamacSys ECAD Model
15912357/958494/2.49/2/5/Capacitor

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r182_107"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1.07) (shapeHeight 1.82))
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
	(patternDef "CAPC3216X180N" (originalName "CAPC3216X180N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r182_107) (pt -1.38, 0) (rotation 0))
			(pad (padNum 2) (padStyleRef r182_107) (pt 1.38, 0) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 30)
			(line (pt -2.065 1.06) (pt 2.065 1.06) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 2.065 1.06) (pt 2.065 -1.06) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 2.065 -1.06) (pt -2.065 -1.06) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -2.065 -1.06) (pt -2.065 1.06) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.6 0.8) (pt 1.6 0.8) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.6 0.8) (pt 1.6 -0.8) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.6 -0.8) (pt -1.6 -0.8) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.6 -0.8) (pt -1.6 0.8) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 0 0.7) (pt 0 -0.7) (width 0.2))
		)
	)
	(symbolDef "GRM31CC71E226ME15L" (originalName "GRM31CC71E226ME15L")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -35 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 500 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 500 mils -35 mils) (rotation 0]) (justify "UpperRight") (textStyleRef "Default"))
		))
		(line (pt 220 mils 100 mils) (pt 220 mils -100 mils) (width 6 mils))
		(line (pt 280 mils 100 mils) (pt 280 mils -100 mils) (width 6 mils))
		(line (pt 200 mils 0 mils) (pt 220 mils 0 mils) (width 6 mils))
		(line (pt 280 mils 0 mils) (pt 300 mils 0 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 350 mils 250 mils) (justify 24) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "GRM31CC71E226ME15L" (originalName "GRM31CC71E226ME15L") (compHeader (numPins 2) (numParts 1) (refDesPrefix C)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "GRM31CC71E226ME15L"))
		(attachedPattern (patternNum 1) (patternName "CAPC3216X180N")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Mouser Part Number" "81-GRM31CC71E226ME5L")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Murata-Electronics/GRM31CC71E226ME15L?qs=pUKx8fyJudCHY0oGjx2nuQ%3D%3D")
		(attr "Manufacturer_Name" "Murata Electronics")
		(attr "Manufacturer_Part_Number" "GRM31CC71E226ME15L")
		(attr "Description" "Multilayer Ceramic Capacitors MLCC - SMD/SMT")
		(attr "Datasheet Link" "http://www.murata.com/~/media/webrenewal/support/library/catalog/products/capacitor/mlcc/c02e.pdf")
		(attr "Height" "1.8 mm")
	)

)
